#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "open.h"

int open_trunc(const char *fn)
{
  return open(fn,O_CREAT | O_WRONLY | O_NONBLOCK | O_TRUNC,0644);
}
