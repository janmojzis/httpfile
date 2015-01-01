#ifndef BYTE_H
#define BYTE_H

extern void byte_zero(void *,long long);
extern void byte_copyr(void *,long long,const void *);
extern void byte_copy(void *,long long,const void *);
extern int byte_isequal(const void *,long long,const void *);
#define byte_equal byte_isequal
#define byte_diff(s,n,t) (!byte_isequal((s),(n),(t)))


extern long long byte_chr(const void *,long long,int);

#endif
