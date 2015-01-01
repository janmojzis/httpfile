/* Public domain. */

#ifndef STR_H
#define STR_H

extern long long str_copy(char *,const char *);
extern long long str_diff(const char *,const char *);
extern long long str_diffn(const char *,const char *,long long);
extern long long str_len(const char *);
extern long long str_chr(const char *,int);
extern long long str_rchr(const char *,int);
extern int str_start(const char *,const char *);

#define str_equal(s,t) (!str_diff((s),(t)))

#endif
