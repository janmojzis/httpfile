#ifndef _AUTH_H____
#define _AUTH_H____

#include "stralloc.h"

extern void auth_init(const char *);
extern int auth_basic(stralloc *, stralloc *, stralloc *);

#endif
