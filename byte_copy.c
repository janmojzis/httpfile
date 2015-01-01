#include "byte.h"

void byte_copy(void *yv,long long ylen,const void *xv)
{
  char *y = yv;
  const char *x = xv;
  while (ylen > 0) { *y++ = *x++; --ylen; }
}

void byte_copyr(void *yv,long long ylen,const void *xv)
{
  char *y = yv + ylen;
  const char *x = xv + ylen;
  while (ylen > 0) { *--y = *--x; --ylen; }
}

