#ifndef GEN_ALLOC_H
#define GEN_ALLOC_H

#define GEN_ALLOC_typedef(ta,type,field,len,a) \
  typedef struct ta { type *field; long long len; long long a; } ta;

#endif
