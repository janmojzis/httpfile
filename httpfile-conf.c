#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "hasdevtcp.h"
#ifdef HASDEVTCP
#include <sys/mkdev.h>
#endif
#include <pwd.h>
#include "strerr.h"
#include "buffer.h"
#include "open.h"
#include "auto_home.h"

#define FATAL "httpfile-conf: fatal: "

void usage(void)
{
  strerr_die1x(100,"httpfile-conf: usage: httpfile-conf acct logacct /public hostname hostip ...");
}

char *dir;
const char *fn;
char buf[1024];
int fd;
buffer b;

void fail(void)
{
  strerr_die6sys(111,FATAL,"unable to create ",dir,"/",fn,": ");
}

void makedir(const char *s)
{
  fn = s;
  if (mkdir(fn,0700) == -1) fail();
}

void start(const char *s)
{
  fn = s;
  fd = open_trunc(fn);
  if (fd == -1) fail();
  buffer_init(&b,buffer_unixwrite,fd,buf,sizeof buf);
}

void outs(const char *s)
{
  if (buffer_puts(&b,s) == -1) fail();
}

void finish(void)
{
  if (buffer_flush(&b) == -1) fail();
  if (fsync(fd) == -1) fail();
  close(fd);
}

void perm(int mode)
{
  if (chmod(fn,mode) == -1) fail();
}

char *user;
char *loguser;
struct passwd *pw;

int main(int argc,char **argv)
{
  umask(022);

  user = argv[1];
  if (!user) usage();
  loguser = argv[2];
  if (!loguser) usage();
  dir = argv[3];
  if (!dir) usage();
  if (dir[0] != '/') usage();

  pw = getpwnam(loguser);
  if (!pw)
    strerr_die3x(111,FATAL,"unknown account ",loguser);

  if (mkdir(dir,0700) == -1)
    strerr_die4sys(111,FATAL,"unable to create ",dir,": ");
  if (chmod(dir,02755) == -1)
    strerr_die4sys(111,FATAL,"unable to set mode of ",dir,": ");
  if (chdir(dir) == -1)
    strerr_die4sys(111,FATAL,"unable to switch to ",dir,": ");

  /* httpd */
  makedir("httpd");
  perm(03700);
  makedir("httpd/log");
  perm(02755);
  makedir("httpd/log/main");
  if (chown("httpd/log/main",pw->pw_uid,pw->pw_gid) == -1)
    strerr_die4sys(111,FATAL,"unable to set owner of ",dir,"/httpd/log/main: ");
  perm(02755);
  start("httpd/log/status");
  finish();
  if (chown("httpd/log/status",pw->pw_uid,pw->pw_gid) == -1)
    strerr_die4sys(111,FATAL,"unable to set owner of ",dir,"/httpd/log/status: ");
  perm(0644);

  start("httpd/run");
  outs("#!/bin/sh\nexec 2>&1\nexec");
  outs(" envuidgid "); outs(user);
  outs(" envdir "); outs(dir); outs("/env sh -c '\n");
  outs("   if [ x\"${IP}\" = x ]; then IP=0.0.0.0; fi\n");
  outs("   if [ x\"${PORT}\" = x ]; then PORT=80; fi\n");
  outs("   if [ x\"${MAXCONNECTIONS}\" = x ]; then MAXCONNECTIONS=100; fi\n");
  outs("   if [ x\"${BACKLOG}\" = x ]; then BACKLOG=50; fi\n");
  outs("   exec softlimit -o20 -d50000");
  outs(" tcpserver -DRHl0 \"-b${BACKLOG}\" \"-c${MAXCONNECTIONS}\" \"${IP}\" \"${PORT}\" ");
  outs(auto_home); outs("/bin/httpfile-httpd\n");
  outs("'");
  finish();
  perm(0755);

  start("httpd/log/run");
  outs("#!/bin/sh\nexec");
  outs(" setuidgid "); outs(loguser);
  outs(" multilog t ./main '-*' '+* * status: *' =status\n");
  finish();
  perm(0755);

  /* httpcurved */
  makedir("httpcurved");
  perm(03700);
  makedir("httpcurved/log");
  perm(02755);
  makedir("httpcurved/log/main");
  if (chown("httpcurved/log/main",pw->pw_uid,pw->pw_gid) == -1)
    strerr_die4sys(111,FATAL,"unable to set owner of ",dir,"/httpcurved/log/main: ");
  perm(02755);
  start("httpcurved/log/status");
  finish();
  if (chown("httpcurved/log/status",pw->pw_uid,pw->pw_gid) == -1)
    strerr_die4sys(111,FATAL,"unable to set owner of ",dir,"/httpcurved/log/status: ");
  perm(0644);

  start("httpcurved/run");
  outs("#!/bin/sh\nexec 2>&1\nexec");
  outs(" envuidgid "); outs(user);
  outs(" envdir "); outs(dir); outs("/env sh -c '\n");
  outs("   if [ x\"${IP}\" = x ]; then IP=0.0.0.0; fi\n");
  outs("   if [ x\"${PORT}\" = x ]; then PORT=80; fi\n");
  outs("   if [ x\"${NAME}\" = x ]; then echo \"\\$NAME not set\";exit 111; fi\n");
  outs("   if [ x\"${KEYDIR}\" = x ]; then echo \"\\$KEYDIR not set\";exit 111; fi\n");
  outs("   if [ x\"${SERVEREXT}\" = x ]; then echo \"\\$SERVEREXT not set\";exit 111; fi\n");
  outs("   exec softlimit -o1000 -d50000");
  outs(" curvecpserver \"${NAME}\" \"${KEYDIR}\" \"${IP}\" \"${PORT}\" \"${SERVEREXT}\" curvecpmessage ");
  outs(auto_home); outs("/bin/httpfile-httpd\n");
  outs("'");
  finish();
  perm(0755);

  start("httpcurved/log/run");
  outs("#!/bin/sh\nexec");
  outs(" setuidgid "); outs(loguser);
  outs(" multilog t ./main '-*' '+* * status: *' =status\n");
  finish();
  perm(0755);

  makedir("env");
  perm(02755);

  start("env/ROOT");
  outs(dir); outs("/file\n");
  finish();
  perm(0644);

  start("env/PORT");
  outs("80");
  finish();
  perm(0644);

  start("env/IP");
  outs("0.0.0.0");
  finish();
  perm(0644);

  start("env/MAXCONNECTIONS");
  outs("100");
  finish();
  perm(0644);

  start("env/BACKLOG");
  outs("50");
  finish();
  perm(0644);

  makedir("file");
  perm(02755);
  makedir("file/plain");
  perm(02755);
  makedir("file/plain/0");
  perm(02755);

#ifdef HASDEVTCP
  makedir("file/etc");
  perm(02755);
  makedir("file/dev");
  perm(02755);
  start("file/etc/netconfig");
  outs("tcp tpi_cots_ord v inet tcp /dev/tcp -\n");
  finish();
  perm(0645);
  umask(000);
  if (mknod("file/dev/tcp",S_IFCHR | 0667,makedev(11,42)) == -1)
    strerr_die4sys(111,FATAL,"unable to create device ",dir,"/file/dev/tcp: ");
  umask(022);
#endif

  if (chdir("file/plain") == -1)
    strerr_die4sys(111,FATAL,"unable to switch to ",dir,"/file: ");
  for (argv += 4;*argv;++argv)
    if (symlink("0",*argv) == -1)
      strerr_die6sys(111,FATAL,"unable to create ",dir,"/file/",*argv,": ");

  _exit(0);
}
