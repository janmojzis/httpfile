#ifndef _ALLOC_H____
#define _ALLOC_H____

#ifndef alloc_ALIGNMENT
#define alloc_ALIGNMENT 16
#endif
#ifndef alloc_STATICSPACE
#define alloc_STATICSPACE (4096 * alloc_ALIGNMENT)
#endif

extern void *alloc(long long);
extern void alloc_free(void *);
extern void alloc_freeall(void);

#endif
