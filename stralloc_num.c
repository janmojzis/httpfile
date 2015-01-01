#include "stralloc.h"

int stralloc_catulonglong0(stralloc *sa,unsigned long long u,long long n)
{
  long long len;
  unsigned long long q;
  char *s;

  len = 1;
  q = u;
  while (q > 9) { ++len; q /= 10; }
  if (len < n) len = n;

  if (!stralloc_readyplus(sa,len)) return 0;
  s = sa->s + sa->len;
  sa->len += len;
  while (len) { s[--len] = '0' + (u % 10); u /= 10; }

  return 1;
}

int stralloc_catlonglong0(stralloc *sa,long long l,long long n)
{
  if (l < 0) {
    if (!stralloc_append(sa,"-")) return 0;
    l = -l;
  }
  return stralloc_catulonglong0(sa,l,n);
}
