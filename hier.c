#include "auto_home.h"

void hier()
{
  h(auto_home,-1,-1,02755);

  d(auto_home,"bin",-1,-1,02755);

  c(auto_home,"bin","httpfile-conf",-1,-1,0755);
  c(auto_home,"bin","httpfile-gzip",-1,-1,0755);
  c(auto_home,"bin","httpfile-httpd",-1,-1,0755);
  c(auto_home,"bin","httpfile-access",-1,-1,0755);
  c(auto_home,"bin","httpfile-setlock",-1,-1,0755);
  c(auto_home,"bin","httpfile-getmtime",-1,-1,0755);
  c(auto_home,"bin","httpfile-setmtime",-1,-1,0755);
}
