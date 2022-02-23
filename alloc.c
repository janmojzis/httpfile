/*
version 20220221
*/

#include <stdlib.h>
#include <errno.h>
#include "log.h"
#include "alloc.h"

static unsigned char space[alloc_STATICSPACE]
    __attribute__((aligned(alloc_ALIGNMENT)));
static unsigned long long avail = sizeof space;
static unsigned long long allocated = 0;

static void **ptr = 0;
static unsigned long long ptrlen = 0;
static unsigned long long ptralloc = 0;

static int ptr_add(void *x) {

    void **newptr;
    unsigned long long i;

    if (!x) return 1;
    if (ptrlen + 1 > ptralloc) {
        while (ptrlen + 1 > ptralloc) ptralloc = 2 * ptralloc + 1;
        newptr = (void **) malloc(ptralloc * sizeof(void *));
        if (!newptr) return 0;
        if (ptr) {
            for (i = 0; i < ptrlen; ++i) newptr[i] = ptr[i];
            free(ptr);
        }
        ptr = newptr;
    }
    ptr[ptrlen++] = x;
    return 1;
}

static int ptr_remove(void *x) {

    unsigned long long i;

    for (i = 0; i < ptrlen; ++i) {
        if (ptr[i] == x) goto ok;
    }
    return 0;
ok:
    --ptrlen;
    ptr[i] = ptr[ptrlen];
    return 1;
}

static void cleanup(void *xv, unsigned long long xlen) {

    volatile unsigned long *x = (volatile unsigned long *) xv;

    xlen /= sizeof(unsigned long);
    while (xlen-- > 0) *x++ = 0;
}

void *alloc(long long norig) {

    unsigned char *x;
    unsigned long long i, n = norig;

    if (norig < 0) {
        log_e3("alloc(", lognum(norig), ") ... failed, < 0");
        goto inval;
    }
    if (n == 0) {
        log_t3("alloc(0), will allocate ", lognum(alloc_ALIGNMENT), " bytes");
        n = alloc_ALIGNMENT;
    }
    n = ((n + alloc_ALIGNMENT - 1) / alloc_ALIGNMENT) * alloc_ALIGNMENT;
    if (n <= avail) {
        avail -= n;
        log_t3("alloc(", lognum(norig), ") ... ok, static");
        return (void *) (space + avail);
    }

    n += alloc_ALIGNMENT;
    allocated += n;

    if (n != (unsigned long long) (size_t) n) {
        log_e3("alloc(", lognum(norig), ") ... failed, size_t overflow");
        goto nomem;
    }

    x = (unsigned char *) malloc(n);
    if (!x) {
        log_e3("alloc(", lognum(norig), ") ... failed, malloc() failed");
        goto nomem;
    }
    cleanup(x, n);

    for (i = 0; i < alloc_ALIGNMENT; ++i) {
        *x++ = n;
        n >>= 8;
    }

    if (!ptr_add(x)) {
        log_e3("alloc(", lognum(norig), ") ... failed, malloc() failed");
        goto nomem;
    }
    log_t5("alloc(", lognum(norig), ") ... ok, using malloc(), total ",
           lognum(allocated), " bytes");
    return (void *) x;
nomem:
    errno = ENOMEM;
    return (void *) 0;
inval:
    errno = EINVAL;
    return (void *) 0;
}

void alloc_free(void *xv) {

    unsigned char *x = xv;
    unsigned long long i, n = 0;

    if (!x) {
        log_w1("alloc_free(0)");
        return;
    }

    if (x >= space)
        if (x < space + sizeof space) return;

    ptr_remove(x);

    for (i = 0; i < alloc_ALIGNMENT; ++i) {
        n <<= 8;
        n |= *--x;
    }

    cleanup(x, n);
    free(x);
}

void alloc_freeall(void) {

    while (ptrlen > 0) { alloc_free(ptr[0]); }
    if (ptr) {
        free(ptr);
        ptr = 0;
        ptrlen = ptralloc = 0;
    }

    cleanup(space, sizeof space);
}
