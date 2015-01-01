#include "buffer.h"
#include "byte.h"
#include "e.h"

static long long oneread(long long (*op)(),int fd,char *buf,long long len)
{
  long long r;

  for (;;) {
    r = op(fd,buf,len);
    if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) continue;
    return r;
  }
}

static long long getthis(buffer *s,char *buf,long long len)
{
  if (len > s->p) len = s->p;
  s->p -= len;
  byte_copy(buf,len,s->x + s->n);
  s->n += len;
  return len;
}

long long buffer_feed(buffer *s)
{
  long long r;

  if (s->p > 0) return s->p;
  r = oneread(s->op,s->fd,s->x,s->n);
  if (r <= 0) return r;
  s->p = r;
  s->n -= r;
  if (s->n > 0) byte_copyr(s->x + s->n,r,s->x);
  return r;
}

long long buffer_bget(buffer *s,char *buf,long long len)
{
  long long r;
 
  if (s->p > 0) return getthis(s,buf,len);
  if (s->n <= len) return oneread(s->op,s->fd,buf,s->n);
  r = buffer_feed(s); if (r <= 0) return r;
  return getthis(s,buf,len);
}

long long buffer_get(buffer *s,char *buf,long long len)
{
  long long r;
 
  if (s->p > 0) return getthis(s,buf,len);
  if (s->n <= len) return oneread(s->op,s->fd,buf,len);
  r = buffer_feed(s); if (r <= 0) return r;
  return getthis(s,buf,len);
}

char *buffer_peek(buffer *s)
{
  return s->x + s->n;
}

void buffer_seek(buffer *s,long long len)
{
  s->n += len;
  s->p -= len;
}
