#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "open.h"

int open_read(const char *fn)
{
  return open(fn,O_RDONLY | O_NONBLOCK);
}
