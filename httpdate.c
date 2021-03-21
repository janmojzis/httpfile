#include <time.h>
#include "httpdate.h"

static char weekday[7][7] = {
  " Sun, ", " Mon, ", " Tue, ", " Wed, ", " Thu, ", " Fri, ", " Sat, "
} ;

static char month[12][6] = {
  " Jan ", " Feb ", " Mar ", " Apr ", " May ", " Jun "
, " Jul ", " Aug ", " Sep ", " Oct ", " Nov ", " Dec "
} ;

int httpdate(stralloc *sa, long long s) {

    time_t secs;
    struct tm *t;

    secs = s;

    t = gmtime(&secs);
    if (!stralloc_copys(sa,weekday[t->tm_wday])) return 0;
    if (!stralloc_catnum0(sa,t->tm_mday,2)) return 0;
    if (!stralloc_cats(sa,month[t->tm_mon])) return 0;
    if (!stralloc_catnum0(sa,1900 + t->tm_year,4)) return 0;
    if (!stralloc_cats(sa," ")) return 0;
    if (!stralloc_catnum0(sa,t->tm_hour,2)) return 0;
    if (!stralloc_cats(sa,":")) return 0;
    if (!stralloc_catnum0(sa,t->tm_min,2)) return 0;
    if (!stralloc_cats(sa,":")) return 0;
    if (!stralloc_catnum0(sa,t->tm_sec,2)) return 0;
    if (!stralloc_cats(sa," GMT")) return 0;
    return 1;
}
