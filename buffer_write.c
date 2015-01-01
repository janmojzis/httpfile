#include <unistd.h>
#include "buffer.h"

long long buffer_unixwrite(int fd,const char *buf,long long len)
{
  if (len > 1048576) len = 1048576;
  return write(fd,buf,len);
}
