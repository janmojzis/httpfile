#include <unistd.h>
#include "buffer.h"

long long buffer_unixread(int fd,char *buf,long long len)
{
  return read(fd,buf,len);
}
