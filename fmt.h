/* Public domain. */

#ifndef _FMT_H____
#define _FMT_H____

#define FMT_LONGLONG 41 /* enough space to hold 2^128 - 1 in decimal, plus \0, plus '-' */
#define FMT_LEN ((char *) 0) /* convenient abbreviation */

extern long long fmt_ulonglong(register char *s,register unsigned long long u);
extern long long fmt_ulonglong0(register char *s,register unsigned long long u, long long n);
extern long long fmt_longlong(char *s, long long u);
extern long long fmt_longlong0(char *s, long long u, long long n);

extern long long fmt_xlonglong(register char *s, register long long u);

#endif

