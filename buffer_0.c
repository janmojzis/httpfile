#include <unistd.h>
#include "buffer.h"

char buffer_0_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(buffer_unixread,0,buffer_0_space,sizeof buffer_0_space);
buffer *buffer_0 = &it;
