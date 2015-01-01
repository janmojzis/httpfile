/* Public domain. */

#ifndef _TRYUINT_H____
#define _TRYUINT_H____

#include <stdio.h>

#define DOIT(bits, target, typedefstr) \
int main(){ \
\
  target x; \
  int i; \
 \
  x = 1; \
  for (i = 0;i < bits;++i) { \
    if (x == 0) return 1; \
    x += x; \
  } \
  if (x != 0) return 1; \
  x -= 1; \
  if (x < 0) return 1; \
 \
  printf("/* Public domain. */\n\n"); \
  printf("#ifndef _UINT%d_H____\n", bits); \
  printf("#define _UINT%d_H____\n\n", bits); \
  printf("typedef %s uint%d;\n\n", typedefstr, bits); \
  if (bits > 8){ \
    printf("extern void uint%d_pack(char *,uint%d);\n", bits, bits); \
    printf("extern void uint%d_pack_big(char *,uint%d);\n", bits, bits); \
    printf("extern void uint%d_unpack(const char *,uint%d *);\n", bits, bits); \
    printf("extern void uint%d_unpack_big(const char *,uint%d *);\n\n", bits, bits); \
  } \
  printf("#endif /* _UINT%d_H____ */\n", bits); \
  return 0; \
}


#endif /* #define _TRYUINT_H____ */
