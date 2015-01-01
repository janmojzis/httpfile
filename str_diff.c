/* Public domain. */

#include "str.h"

long long str_diff(register const char *s,register const char *t)
{
  register char x;

  for (;;) {
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
    x = *s; if (x != *t) break; if (!x) break; ++s; ++t;
  }
  return ((long long)(unsigned long long)(unsigned char) x)
       - ((long long)(unsigned long long)(unsigned char) *t);
}
