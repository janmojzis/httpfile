#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <utime.h>
#include "scan.h"
#include "strerr.h"

#define FATAL "httpfile-setmtime: fatal: "

void die_usage(void){strerr_die1x(100,"httpfile-setmtime: usage: httpfile-setmtime file timestamp");}
void die_utime(const char *str){strerr_die4sys(111,FATAL,"unable to set modification time to file '",str,"': ");}

char *fn;
char *ustr;
long long u;
struct utimbuf ut;

int main(int argc,char **argv)
{
  fn = argv[1];
  if (!fn) die_usage();

  ustr = argv[2];
  if (!ustr) die_usage();
  scan_longlong(ustr,&u);

  ut.actime = ut.modtime = u;
  if (utime(fn,&ut) == -1) die_utime(fn);
  _exit(0);
}
