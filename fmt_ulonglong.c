#include "fmt.h"

long long fmt_ulonglong(register char *s,register unsigned long long u)
{
  register long long len; register unsigned long long q;
  len = 1; q = u;
  while (q > 9) { ++len; q /= 10; }
  if (s) {
    s += len;
    do { *--s = '0' + (u % 10); u /= 10; } while(u); /* handles u == 0 */
  }
  return len;
}


long long fmt_ulonglong0(register char *s,register unsigned long long u, long long n)
{
  long long len;
  len = fmt_ulonglong(FMT_LEN,u);
  while (len < n) { if (s) *s++ = '0'; ++len; }
  if (s) fmt_ulonglong(s,u);
  return len;
}
