/* Public domain. */

#include "str.h"

long long str_len(const char *str) {

    const char *s;

    for (s = str; *s; ++s);
    return (s - str);
}

