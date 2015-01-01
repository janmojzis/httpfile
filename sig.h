/* Public domain. */

#ifndef SIG_H
#define SIG_H

#include <signal.h>

extern void sig_catch(int,void (*)());
#define sig_ignore(s) (sig_catch((s),SIG_IGN))
#define sig_uncatch(s) (sig_catch((s),SIG_DFL))

extern void sig_block(int);
extern void sig_unblock(int);
extern void sig_blocknone(void);
extern void sig_pause(void);

extern void sig_dfl(int);

#endif
