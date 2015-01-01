#include "case.h"

long long case_diffb(register const char *s,register long long len,register const char *t)
{
  register unsigned char x;
  register unsigned char y;

  while (len > 0) {
    --len;
    x = *s++ - 'A';
    if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = *t++ - 'A';
    if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if (x != y)
      return ((long long)(unsigned long long) x) - ((long long)(unsigned long long) y);
  }
  return 0;
}
