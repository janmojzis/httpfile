#ifndef _ALLOC_H____
#define _ALLOC_H____

#ifndef alloc_ALIGNMENT
#define alloc_ALIGNMENT 16
#endif
#ifndef alloc_STATICSPACE
#define alloc_STATICSPACE (8192 * alloc_ALIGNMENT)
#endif
#ifndef alloc_LIMIT
#define alloc_LIMIT 4611686018427387904ULL
#endif

extern void *alloc(unsigned long long);
extern void alloc_free(void *);
extern void alloc_freeall(void);

#endif
