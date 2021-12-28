/* Public domain. */

#ifndef CASE_H
#define CASE_H

extern void case_lowers(char *);
extern void case_lowerb(char *, long long);
extern long long case_diffs(const char *, const char *);
extern long long case_diffb(const char *, long long, const char *);
extern int case_starts(const char *, const char *);
extern int case_startb(const char *, long long, const char *);

#define case_equals(s, t) (!case_diffs((s), (t)))

#endif
