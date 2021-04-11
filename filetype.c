#include "filetype.h"
#include "str.h"
#include "case.h"

int filetype(const char *fn,stralloc *contenttype)
{
  const char *x;
  const char *result;
  long long i;
  char ch;

  if (!stralloc_copys(contenttype, "")) return 0;

  x = fn + str_rchr(fn,'.');
  if (x[str_chr(x,'=')])
    for (i = 0;(i < 100) && (ch = x[i]);++i) {
      if ((ch != '=') && (ch != '-') && (ch != ':'))
        if ((ch < 'a') || (ch > 'z'))
          if ((ch < '0') || (ch > '9'))
            continue;
      if (ch == '=') ch = '/';
      if (ch == ':') ch = '.';
      if (!stralloc_append(contenttype, &ch)) return 0;
    }
  else {
    result = "text/plain";
    /* web content */
    if (case_equals(x,".html")) result = "text/html";
    else if (case_equals(x,".css")) result = "text/css";
    else if (case_equals(x,".js"))  result = "application/x-javascript";
    else if (case_equals(x,".xml")) result = "text/xml";
    else if (case_equals(x,".rss")) result = "text/xml";
    /* archive + commpressed */
    else if (case_equals(x,".tar")) result = "application/x-tar";
    else if (case_equals(x,".gz"))  result = "application/x-gzip";
    else if (case_equals(x,".bz2")) result = "application/x-bzip2";
    else if (case_equals(x,".7z"))  result = "application/x-7z-compressed";
    else if (case_equals(x,".xz"))  result = "application/x-xz";
    else if (case_equals(x,".tgz"))  result = "application/x-gtar-compressed";
    /* documents */
    else if (case_equals(x,".dvi")) result = "application/x-dvi";
    else if (case_equals(x,".ps"))  result = "application/postscript";
    else if (case_equals(x,".pdf")) result = "application/pdf";
    /* images */
    else if (case_equals(x,".ico")) result = "image/x-icon";
    else if (case_equals(x,".gif")) result = "image/gif";
    else if (case_equals(x,".jpg")) result = "image/jpeg";
    else if (case_equals(x,".jpeg")) result = "image/jpeg";
    else if (case_equals(x,".png")) result = "image/png";
    /* video */
    else if (case_equals(x,".mpg")) result = "video/mpeg";
    else if (case_equals(x,".mpeg")) result = "video/mpeg";
    else if (case_equals(x,".flv")) result = "video/x-flv";
    else if (case_equals(x,".avi")) result = "video/x-msvideo";
    else if (case_equals(x,".mp4")) result = "video/mp4";
    else if (case_equals(x,".ogg")) result = "video/ogg";
    else if (case_equals(x,".mov")) result = "video/quicktime";
    else if (case_equals(x,".webm")) result = "video/webm";

    if (!stralloc_cats(contenttype, result)) return 0;
  }
  return 1;
}
