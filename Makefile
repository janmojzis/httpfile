CC?=cc
CFLAGS+=-W -Wall -Os -fPIC -fwrapv -Wall
LDFLAGS+=

all:  httpfile utime

alloc.o: alloc.c alloc.h log.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c alloc.c

case_diffb.o: case_diffb.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c case_diffb.c

case_diffs.o: case_diffs.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c case_diffs.c

case_lowerb.o: case_lowerb.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c case_lowerb.c

case_startb.o: case_startb.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c case_startb.c

droproot.o: droproot.c droproot.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c droproot.c

e.o: e.c e.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c e.c

file.o: file.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c file.c

filetype.o: filetype.c filetype.h stralloc.h str.h case.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c filetype.c

getuidgid.o: getuidgid.c getuidgid.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c getuidgid.c

hostparse.o: hostparse.c stralloc.h hostparse.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c hostparse.c

httpdate.o: httpdate.c httpdate.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c httpdate.c

httpfile.o: httpfile.c stralloc.h pathdecode.h hostparse.h httpdate.h \
 seconds.h percent.h case.h log.h str.h filetype.h file.h droproot.h \
 alloc.h e.h timeoutwrite.h rangeparser.h getuidgid.h limits.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c httpfile.c

limits.o: limits.c limits.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c limits.c

log.o: log.c e.h log.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c log.c

milliseconds.o: milliseconds.c milliseconds.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c milliseconds.c

pathdecode.o: pathdecode.c pathdecode.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c pathdecode.c

percent.o: percent.c percent.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c percent.c

rangeparser.o: rangeparser.c rangeparser.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c rangeparser.c

seconds.o: seconds.c seconds.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c seconds.c

stralloc.o: stralloc.c alloc.h e.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c stralloc.c

str.o: str.c str.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c str.c

timeoutwrite.o: timeoutwrite.c e.h milliseconds.h timeoutwrite.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c timeoutwrite.c

utime.o: utime.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c utime.c

httpfile: httpfile.o  alloc.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o droproot.o e.o file.o filetype.o getuidgid.o hostparse.o httpdate.o limits.o log.o milliseconds.o pathdecode.o percent.o rangeparser.o seconds.o stralloc.o str.o timeoutwrite.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o httpfile httpfile.o  alloc.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o droproot.o e.o file.o filetype.o getuidgid.o hostparse.o httpdate.o limits.o log.o milliseconds.o pathdecode.o percent.o rangeparser.o seconds.o stralloc.o str.o timeoutwrite.o $(LDFLAGS)

utime: utime.o  alloc.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o droproot.o e.o file.o filetype.o getuidgid.o hostparse.o httpdate.o limits.o log.o milliseconds.o pathdecode.o percent.o rangeparser.o seconds.o stralloc.o str.o timeoutwrite.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o utime utime.o  alloc.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o droproot.o e.o file.o filetype.o getuidgid.o hostparse.o httpdate.o limits.o log.o milliseconds.o pathdecode.o percent.o rangeparser.o seconds.o stralloc.o str.o timeoutwrite.o $(LDFLAGS)

rts.out:  httpfile utime rts.tests
	sh rts.tests > rts.out

rts: rts.exp rts.out
	diff rts.exp rts.out

clean:
	rm -f *.o  httpfile utime rts.out

