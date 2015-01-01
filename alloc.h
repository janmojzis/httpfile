/* Public domain. */

#ifndef ALLOC_H
#define ALLOC_H

extern void *alloc(long long);
extern void alloc_free(void *x);
extern int alloc_re(void **x, long long, long long);

#endif
