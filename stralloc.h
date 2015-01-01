#ifndef STRALLOC_H
#define STRALLOC_H

#include "gen_alloc.h"

GEN_ALLOC_typedef(stralloc,char,s,len,a)

extern int stralloc_ready(stralloc *,long long);
extern int stralloc_readyplus(stralloc *,long long);
extern int stralloc_copy(stralloc *,const stralloc *);
extern int stralloc_cat(stralloc *,const stralloc *);
extern int stralloc_copys(stralloc *,const char *);
extern int stralloc_cats(stralloc *,const char *);
extern int stralloc_copyb(stralloc *,const char *,long long);
extern int stralloc_catb(stralloc *,const char *,long long);
extern int stralloc_append(stralloc *,const char *); /* beware: this takes a pointer to 1 char */
extern int stralloc_starts(stralloc *,const char *);

#define stralloc_0(sa) stralloc_append(sa,"")

extern int stralloc_catulonglong0(stralloc *,unsigned long long,long long);
extern int stralloc_catlonglong0(stralloc *,long long,long long);
#define stralloc_catlonglong(sa,l) (stralloc_catlonglong0((sa),(l),0))
#define stralloc_catlong(sa,l) (stralloc_catlonglong0((sa),(l),0))
#define stralloc_catint(sa,l) (stralloc_catlonglong0((sa),(l),0))

#endif
