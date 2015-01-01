#ifndef BASE64_H
#define BASE64_H

#include "stralloc.h"

extern int base64_decode(stralloc *out, const unsigned char *in, long long inlen);
extern int base64_encode(stralloc *out, const unsigned char *in, long long inlen);

#endif
