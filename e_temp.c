#include <errno.h>
#include "e.h"

#define X(n) if (e == n) return 1;

int e_temp(int e)
{
#ifndef EINTR
  X(EINTR)
#endif
#ifndef ENOMEM
  X(ENOMEM)
#endif
#ifndef ETXTBSY
  X(ETXTBSY)
#endif
#ifndef EIO
  X(EIO)
#endif
#ifndef ETIMEDOUT
  X(ETIMEDOUT)
#endif
#ifndef EWOULDBLOCK
  X(EWOULDBLOCK)
#endif
#ifndef EAGAIN
  X(EAGAIN)
#endif
#ifdef EDEADLK
  X(EDEADLK)
#endif
#ifdef EBUSY
  X(EBUSY)
#endif
#ifdef ENFILE
  X(ENFILE)
#endif
#ifdef EMFILE
  X(EMFILE)
#endif
#ifdef EFBIG
  X(EFBIG)
#endif
#ifdef ENOSPC
  X(ENOSPC)
#endif
#ifdef ENETDOWN
  X(ENETDOWN)
#endif
#ifdef ENETUNREACH
  X(ENETUNREACH)
#endif
#ifdef ENETRESET
  X(ENETRESET)
#endif
#ifdef ECONNABORTED
  X(ECONNABORTED)
#endif
#ifdef ECONNRESET
  X(ECONNRESET)
#endif
#ifdef ENOBUFS
  X(ENOBUFS)
#endif
#ifdef ETOOMANYREFS
  X(ETOOMANYREFS)
#endif
#ifdef ECONNREFUSED
  X(ECONNREFUSED)
#endif
#ifdef EHOSTDOWN
  X(EHOSTDOWN)
#endif
#ifdef EHOSTUNREACH
  X(EHOSTUNREACH)
#endif
#ifdef EPROCLIM
  X(EPROCLIM)
#endif
#ifdef EUSERS
  X(EUSERS)
#endif
#ifdef EDQUOT
  X(EDQUOT)
#endif
#ifdef ESTALE
  X(ESTALE)
#endif
#ifdef ENOLCK
  X(ENOLCK)
#endif
  return 0;
}
