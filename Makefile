CC?=cc
CFLAGS+=-W -Wall -Os -fPIC -fwrapv -pedantic

BINARIES=httpfile
BINARIES+=utime

all: $(BINARIES)

alloc.o: alloc.c randombytes.h alloc.h log.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c alloc.c

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
 alloc.h e.h timeoutwrite.h rangeparser.h getuidgid.h randombytes.h \
 limits.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c httpfile.c

limits.o: limits.c log.h limits.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c limits.c

log.o: log.c e.h log.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c log.c

milliseconds.o: milliseconds.c milliseconds.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c milliseconds.c

pathdecode.o: pathdecode.c pathdecode.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c pathdecode.c

percent.o: percent.c percent.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c percent.c

randombytes.o: randombytes.c randombytes.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c randombytes.c

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

OBJECTS=alloc.o
OBJECTS+=case_diffs.o
OBJECTS+=case_lowerb.o
OBJECTS+=case_startb.o
OBJECTS+=droproot.o
OBJECTS+=e.o
OBJECTS+=file.o
OBJECTS+=filetype.o
OBJECTS+=getuidgid.o
OBJECTS+=hostparse.o
OBJECTS+=httpdate.o
OBJECTS+=limits.o
OBJECTS+=log.o
OBJECTS+=milliseconds.o
OBJECTS+=pathdecode.o
OBJECTS+=percent.o
OBJECTS+=randombytes.o
OBJECTS+=rangeparser.o
OBJECTS+=seconds.o
OBJECTS+=stralloc.o
OBJECTS+=str.o
OBJECTS+=timeoutwrite.o

httpfile: httpfile.o $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o httpfile httpfile.o $(OBJECTS) $(LDFLAGS)

utime: utime.o $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o utime utime.o $(OBJECTS) $(LDFLAGS)


rts.out: $(BINARIES) rts.tests
	sh rts.tests > rts.out

test: rts.exp rts.out
	cmp rts.exp rts.out

clean:
	rm -f *.o $(BINARIES) rts.out

