#ifndef FILE_H
#define FILE_H

extern int file_open(char *,long long *,long long *);
extern void file_log(char *);
extern int file_seek(int, long long);
extern long long file_read(int, char *, long long);

#endif
