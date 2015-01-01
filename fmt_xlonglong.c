#include "fmt.h"

long long fmt_xlonglong(register char *s, register long long u)
{
  register long long len; register long long q; register char c;
  len = 1; q = u;
  while (q > 15) { ++len; q /= 16; }
  if (s) {
    s += len;
    do { c = '0' + (u & 15); if (c > '0' + 9) c += 'a' - '0' - 10;
    *--s = c; u /= 16; } while(u);
  }
  return len;
}
