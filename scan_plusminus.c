#include "scan.h"

long long scan_plusminus(register const char *s, register int *sign)
{
  if (*s == '+') { *sign = 1; return 1; }
  if (*s == '-') { *sign = -1; return 1; }
  *sign = 1; return 0;
}
