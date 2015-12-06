#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "pathdecode.h"
#include "file.h"
#include "filetype.h"
#include "percent.h"
#include "stralloc.h"
#include "fmt.h"
#include "case.h"
#include "str.h"
#include "byte.h"
#include "seconds.h"
#include "httpdate.h"
#include "timeoutread.h"
#include "timeoutwrite.h"
#include "buffer.h"
#include "e.h"
#include "getln.h"
#include "env.h"
#include "scan.h"
#include "strerr.h"
#include "sig.h"
#include "droproot.h"
#ifdef AUTH
#include "auth.h"
#endif

#define FATAL "httpfile-httpd: fatal: "

#define REQUESTTIMEOUT 20
#define READTIMEOUT    30
#define WRITETIMEOUT   30

static void barftimeout(void);

void die_nomem(void){strerr_die2x(21,FATAL,"out of memory");}
void die_read(void){strerr_die2sys(23,FATAL,"unable to read file: ");}
void die_open(const char *str){strerr_die4sys(23,FATAL,"unable to open file '",str,"': ");}

long long safewrite(int fd,char *buf,long long len)
{
  long long  r;
  r = timeoutwrite(WRITETIMEOUT,fd,buf,len);
  if (r <= 0) _exit(0);
  return r;
}

char outbuf[1024];
buffer out = BUFFER_INIT(safewrite,1,outbuf,sizeof outbuf);

void out_put(const char *s,int len)
{
  buffer_put(&out,s,len);
}

void out_puts(const char *s)
{
  buffer_puts(&out,s);
}

void out_flush(void)
{
  buffer_flush(&out);
}

char strnum[FMT_LONGLONG];

stralloc protocol = {0};
int protocolnum = 0;
stralloc method = {0};
stralloc url = {0};
stralloc host = {0};
stralloc path = {0};
stralloc qs   = {0};
stralloc ims = {0};
stralloc auth = {0};
stralloc range = {0};
int flagbody = 1;
int flaggzip = 0;

char filebuf[1024];

stralloc nowstr = {0};
long long mtime;
long long mtimegz;
stralloc mtimestr = {0};

void header(const char *code, const char *message)
{
  if (protocolnum == 1)
    out_puts("HTTP/1.0 ");
  else
    out_puts("HTTP/1.1 ");
  out_puts(code);
  out_puts(message);
  out_puts("\r\nServer: httpfile\r\nDate:");
  if (!httpdate(&nowstr,seconds())) die_nomem();
  out_put(nowstr.s,nowstr.len);
  out_puts("\r\nAccept-Ranges: bytes\r\n");
}


void barf2(const char *code, const char *message, int realm)
{
  if (protocolnum > 0) {
    header(code,message);
    if (realm) {
      out_puts("WWW-Authenticate: Basic realm=\"authorization required\"\r\n");
    }
    out_puts("Content-Length: ");
    out_put(strnum,fmt_longlong(strnum,str_len(message) + 28));
    out_puts("\r\n");
    if (protocolnum == 2)
      out_puts("Connection: close\r\n");
    out_puts("Content-Type: text/html\r\n\r\n");
  }
  if (flagbody) {
    out_puts("<html><body>");
    out_puts(message);
    out_puts("</body></html>\r\n");
  }
  out_flush();
  if (protocolnum >= 2) {
    shutdown(1,1);
    sleep(1); /* XXX */
  }
  _exit(0);
}

void barf(const char *code, const char *message) {
    barf2(code, message, 0);
}

stralloc fn = {0};
stralloc fngz = {0};
stralloc contenttype = {0};


stralloc redirurl;
void redirect(void)
{
  if (!stralloc_copy(&redirurl,&url)) die_nomem();
  if (!stralloc_cats(&redirurl, "/")) die_nomem();
  if (protocolnum > 0) {
    header("301 ", "moved permanently");
    out_puts("Content-Length: 0\r\nLocation: ");
    out_put(redirurl.s, redirurl.len);
    out_puts("\r\nContent-Type: text/html\r\n\r\n");
  }
  out_flush();
  if (protocolnum < 2) _exit(0);
}



void get(void)
{
  long long length;
  long long lengthgz;
  long long firstbyte = 0;
  long long lastbyte = 0;
  long long curbyte = 0;
  long long ll;
  int fd;
  int fdgz = -1;
  long long r;
  struct stat st;
  int ret;

  host.len = byte_chr(host.s,host.len,':');
  if (!host.len) {
    if (protocolnum > 1)
      barf("400 ","HTTP/1.1 requests must include a host name");
    if (!stralloc_copys(&host,"0")) die_nomem();
  }
  case_lowerb(host.s,host.len);
  percent(&path);

  /* query string */
  r = byte_chr(path.s,path.len, '?');
  path.len = r;

#ifdef AUTH
  ret = auth_basic(&host, &path,&auth);
  if (ret == -1) die_nomem();
  if (ret) {
      if (auth.len <= 0) {
          barf2("401 ", "unauthorized", 1);
      }
      else {
          barf("403 ", "unauthorized");
      }
  }
#endif

  if (!stralloc_copys(&fn,"./plain/")) die_nomem();
  if (!stralloc_cat(&fn,&host)) die_nomem();
  if (!stralloc_cats(&fn,"/")) die_nomem();
  if (!stralloc_cat(&fn,&path)) die_nomem();
  pathdecode(&fn);
  if (!stralloc_0(&fn)) die_nomem();

  if (range.len > 0) flaggzip = 0;

  if (flaggzip) {
    if (!stralloc_copys(&fngz,"./gzip/")) die_nomem();
    if (!stralloc_cat(&fngz,&host)) die_nomem();
    if (!stralloc_cats(&fngz,"/")) die_nomem();
    if (!stralloc_cat(&fngz,&path)) die_nomem();
    pathdecode(&fngz);
    if (!stralloc_0(&fngz)) die_nomem();
    /* Only call file_open if we can stat the .gz file*/
    if (!stat(fngz.s,&st)) {
       fdgz = file_open(fngz.s,&mtimegz,&lengthgz);
    }
    if (fdgz < 0)
      flaggzip = 0;
  }

  fd = file_open(fn.s,&mtime,&length);
  if (fd == -1) {
    switch(errno) {
      case EISDIR:
        redirect(); return;
      case EACCES:
        barf("403 ",e_str(errno));break;
      default:
        barf("404 ",e_str(errno));break;
    }
  }

  if (flaggzip) {
    if (mtimegz < mtime) {
      flaggzip = 0;
      close(fdgz);
    } else {
      close(fd);
      fd = fdgz;
      length = lengthgz;
    } 
  }
  if (flaggzip) {
    file_log(fngz.s);
  }
  else {
    file_log(fn.s);
  }
  if (!filetype(fn.s,&contenttype)) die_nomem();

  firstbyte = 0;
  lastbyte  = length;
  if (range.len > 0) {

    if (length <= 0)
      barf("416 ", "Requested Range Not Satisfiable");

    do {
      if (!stralloc_0(&range)) die_nomem();
      if (!stralloc_0(&range)) die_nomem();
      ll = scan_longlong(range.s, &firstbyte);
      if (ll <= 0) { range.len = 0; firstbyte = 0; lastbyte = length; break; }
      if (ll == 1 && range.s[0] == '-') barf("416 ", "Requested Range Not Satisfiable");

      if (range.s[0] == '-') {
          ll = scan_longlong(range.s, &firstbyte);
          if (ll <= 1) { range.len = 0; firstbyte = 0; lastbyte = length; break; }
          lastbyte  = length;
          firstbyte += length;
          if (firstbyte < 0) { range.len = 0; firstbyte = 0; lastbyte = length; break; }
      }
      else {
          ll = scan_longlong(range.s, &firstbyte);
          if (ll <= 0) { range.len = 0; firstbyte = 0; lastbyte = length; break; }
          ll = scan_longlong(range.s + ll + 1, &lastbyte);
          if (ll <= 0) lastbyte = length - 1;
      }

      if (lastbyte >= length) lastbyte = length - 1;
      lastbyte += 1;
      if (firstbyte >= lastbyte)
        barf("416 ", "Requested Range Not Satisfiable");
    } while(0);
  }

  if (protocolnum > 0) {
    if (!httpdate(&mtimestr,mtime)) die_nomem();
    if ((ims.len < mtimestr.len) || byte_diff(mtimestr.s,mtimestr.len,ims.s)) {
      if (range.len > 0) {
        header("206 ", "Partial content");
        out_puts("Content-Range: bytes ");
        out_put(strnum, fmt_longlong(strnum, firstbyte));
        out_puts("-");
        out_put(strnum, fmt_longlong(strnum, lastbyte - 1));
        out_puts("/");
        out_put(strnum, fmt_longlong(strnum, length));
        out_puts("\r\n");
      }
      else {
        header("200 ","OK");
      }
    }
    else {
      header("304 ","OK");
      flagbody = 0;
    }
    if (seconds() > mtime + 60) {
      out_puts("Last-Modified:");
      out_put(mtimestr.s,mtimestr.len);
      out_puts("\r\n");
      out_puts("Expires: -1\r\nCache-Control: must-revalidate, private\r\n");
    }
    /* if (!filetype(fn.s,&contenttype)) die_nomem(); */
    out_put(contenttype.s,contenttype.len);
    if (flaggzip)
      out_puts("Content-Encoding: gzip\r\n");
    if (protocolnum >= 2)
      out_puts("Transfer-Encoding: chunked\r\n");
    else {
      out_puts("Content-Length: ");
      out_put(strnum, fmt_longlong(strnum, lastbyte - firstbyte)); /* XXX: could change */
      out_puts("\r\n");
    }
    out_puts("\r\n");
  }

  if (protocolnum < 2) {
    if (flagbody) {
      if (firstbyte) {
        if (file_seek(fd, firstbyte) == -1) die_read();
        curbyte = firstbyte;
      }
      for (;;) {
        r = file_read(fd, filebuf, sizeof filebuf);
        if (r == -1) die_read();
        if (r == 0) break;
        if (curbyte + r > lastbyte) r = lastbyte - curbyte;
        out_put(filebuf, r);
        curbyte += r;
        if (curbyte == lastbyte) break;
      }
    }
    out_flush();
    _exit(0);
  }

  if (flagbody) {
    for (;;) {
      r = file_read(fd,filebuf,sizeof filebuf);
      if (r == -1) die_read();
      if (r == 0) { out_puts("0\r\n\r\n"); out_flush(); break; }
      out_put(strnum,fmt_xlonglong(strnum, r));
      out_puts("\r\n");
      out_put(filebuf,r);
      out_puts("\r\n");
    }
  }

  close(fd);
}

stralloc field = {0};
stralloc line = {0};

long long saferead(int fd,char *buf,long long len)
{
  long long r;
  out_flush();
  r = timeoutread(READTIMEOUT,fd,buf,len);
  if (r == -1) if (errno == ETIMEDOUT) barftimeout();
  if (r <= 0) _exit(0);
  return r;
}

char inbuf[512];
buffer in = BUFFER_INIT(saferead,0,inbuf,sizeof inbuf);

void readline(void)
{
  int match;

  if (getln(&in,&line,&match,'\n') == -1) die_nomem();
  if (!match) _exit(0);
  if (line.len > 0 && (line.s[line.len - 1] == '\n')) --line.len;
  if (line.len > 0 && (line.s[line.len - 1] == '\r')) --line.len;
}

void barftimeout(void) {
  errno = ETIMEDOUT;
  barf("408 ",e_str(errno));
}

void doit()
{
  long long i;
  long long spaces;

  sig_ignore(SIGPIPE);
  sig_catch(SIGALRM,barftimeout);

  alarm(REQUESTTIMEOUT);

  for (;;) {
    readline();

    if (line.len <= 0) continue;

    if (!stralloc_copys(&method,"")) die_nomem();
    if (!stralloc_copys(&url,"")) die_nomem();
    if (!stralloc_copys(&host,"")) die_nomem();
    if (!stralloc_copys(&path,"")) die_nomem();
    if (!stralloc_copys(&qs,"")) die_nomem();
    if (!stralloc_copys(&protocol,"")) die_nomem();
    if (!stralloc_copys(&ims,"")) die_nomem();
    if (!stralloc_copys(&auth,"")) die_nomem();
    if (!stralloc_copys(&range,"")) die_nomem();
    protocolnum = 2;

    spaces = 0;
    for (i = 0;i < line.len;++i)
      if (line.s[i] == ' ') {
        if (!i || (line.s[i - 1] != ' ')) {
          ++spaces;
          if (spaces >= 3) break;
        }
      }
      else
        switch(spaces) {
          case 0:
            if (!stralloc_append(&method,line.s + i)) die_nomem();
            break;
          case 1:
            if (!stralloc_append(&url,line.s + i)) die_nomem();
            break;
          case 2:
            if (!stralloc_append(&protocol,line.s + i)) die_nomem();
            break;
        }

    if (protocol.len <= 0)
      protocolnum = 0;
    else {
      if (!stralloc_0(&protocol)) die_nomem();
      if (case_equals(protocol.s,"http/1.0"))
        protocolnum = 1; /* if client uses http/001.00, tough luck */
    }

    if (!stralloc_0(&method)) die_nomem();
    flagbody = 1;
    if (str_equal(method.s,"HEAD"))
      flagbody = 0;
    else if (!str_equal(method.s,"GET"))
      barf("501 ","method not implemented");

    if (case_startb(url.s,url.len,"http://")) {
      if (!stralloc_copyb(&host,url.s + 7,url.len - 7)) die_nomem();
      i = byte_chr(host.s,host.len,'/');
      if (!stralloc_copyb(&path,host.s + i,host.len - i)) die_nomem();
      host.len = i;
    }
    else
      if (!stralloc_copy(&path,&url)) die_nomem();

    if (path.len <= 0 || (path.s[path.len - 1] == '/'))
      if (!stralloc_cats(&path,"index.html")) die_nomem();

    flaggzip = 0;
    if (protocolnum > 0) {
      if (!stralloc_copys(&field,"")) die_nomem();
      for (;;) {
        readline();
        if (line.len <= 0 || ((line.s[0] != ' ') && (line.s[0] != '\t'))) {
          if (case_startb(field.s,field.len,"content-length:"))
            barf("501 ","I do not accept messages");
          if (case_startb(field.s,field.len,"transfer-encoding:"))
            barf("501 ","I do not accept messages");
          if (case_startb(field.s,field.len,"expect:"))
            barf("417 ","I do not accept Expect");
          if (case_startb(field.s,field.len,"if-match:"))
            barf("412 ","I do not accept If-Match");
          if (case_startb(field.s,field.len,"if-none-match:"))
            barf("412 ","I do not accept If-None-Match");
          if (case_startb(field.s,field.len,"if-unmodified-since:"))
            barf("412 ","I do not accept If-Unmodified-Since");
          if (case_startb(field.s,field.len,"host:"))
            if (host.len <= 0)
              for (i = 5;i < field.len;++i)
                if (field.s[i] != ' ')
                  if (field.s[i] != '\t')
                    if (!stralloc_append(&host,&field.s[i])) die_nomem();
          if (case_startb(field.s,field.len,"if-modified-since:"))
	    if (!stralloc_copyb(&ims,field.s + 18,field.len - 18)) die_nomem();
          if (case_startb(field.s,field.len,"authorization: basic "))
            if (!stralloc_copyb(&auth,field.s + 21,field.len - 21)) die_nomem();
	  if (case_startb(field.s,field.len,"accept-encoding:"))
	    for (i = 16; i < field.len-3; i++)
	      if (case_startb(field.s+i,field.len-i,"gzip")) 
	        flaggzip = 1;
          if (protocolnum == 1 && case_startb(field.s, field.len, "range: bytes=")) {
            if (byte_chr(field.s + 13, field.len - 13, ',') == field.len - 13) {
              if (!stralloc_copyb (&range, field.s + 13, field.len - 13)) die_nomem();
            }
          }

          field.len = 0;
        }
        if (line.len <= 0) break;
        if (!stralloc_cat(&field,&line)) die_nomem();
      }
    }
    alarm(0);
    get();
    alarm(REQUESTTIMEOUT);
  }
}

int main(int argc,char **argv)
{

#ifdef AUTH
  auth_init(env_get("ACCESSFILE"));
#endif

  droproot(FATAL);
  doit();
  return 111;
}


