/* Public domain. */

#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "e.h"
#include "alloc.h"

#define ALIGNMENT 16 /* XXX: assuming that this alignment is enough */
#define SPACE 8192 /* must be multiple of ALIGNMENT */

typedef union { char irrelevant[ALIGNMENT]; double d; } aligned;
static aligned realspace[SPACE / ALIGNMENT];
#define space ((char *) realspace)
static long long avail = SPACE; /* multiple of ALIGNMENT; 0<=avail<=SPACE */

static long long allocated = 0;
static long long limit     = 0;


static void getlimit(void) {

#ifdef RLIMIT_DATA
    struct rlimit r;
    if (getrlimit(RLIMIT_DATA, &r) == 0) {
        if (r.rlim_cur > r.rlim_max)
            r.rlim_cur = r.rlim_max;
        limit = (long long)r.rlim_cur;
    }
#endif
    if (limit <= 0)
        limit = 1073741823; /* 1G */
    return;
}


void *alloc(long long n) {

    unsigned char *x;

    if (n < 0) { errno = ENOMEM; return (void *)0; }
#if 0
    if (sizeof(size_t) < 8){
        if (n > 4000000000LL) { errno = ENOMEM; return (void *)0; } /* 32bit hard limit, slightly less than 4G*/
    }
    else{
        if (n > 1000000000000LL) { errno = ENOMEM; return (void *)0; } /* hard limit, slightly less than 1T */
    }
#else
    if (n > 1073741823) { errno = ENOMEM; return (void *)0; }
#endif
    if (n == 0) n = 1;

    n = ALIGNMENT + n - (n & (ALIGNMENT - 1));
    if (n <= avail) { avail -= n; return (void *)(space + avail); }
    n += 5;

    if (limit <= 0) getlimit();
    if (allocated + n > limit) { errno = ENOMEM; return (void *)0; }

    x = malloc(n);
    if (!x) { errno = ENOMEM; return (void *)0; }

    allocated += n;
    *x++ = n; n >>= 8;
    *x++ = n; n >>= 8;
    *x++ = n; n >>= 8;
    *x++ = n; n >>= 8;
    *x++ = n; n >>= 8;

    return (void *)x;
}

void alloc_free(void *xv) {
    
    unsigned char *p;
    long long n;
    char *x = xv;

    if (x >= space)
        if (x < space + SPACE)
            return;

    p = (unsigned char *)x - 5;
    n = p[4];
    n <<= 8; n |= p[3];
    n <<= 8; n |= p[2];
    n <<= 8; n |= p[1];
    n <<= 8; n |= p[0];
    allocated -= n;

    return free(x - 5);
}
