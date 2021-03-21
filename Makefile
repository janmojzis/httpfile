CC=cc
CFLAGS+=-W -Wall -Os -fPIC -fwrapv -Wall
LDFLAGS+=

all:  httpfile utime

alloc.o: alloc.c alloc.h log.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c alloc.c

byte_chr.o: byte_chr.c byte.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c byte_chr.c

byte_isequal.o: byte_isequal.c byte.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c byte_isequal.c

case_diffb.o: case_diffb.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c case_diffb.c

case_diffs.o: case_diffs.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c case_diffs.c

case_lowerb.o: case_lowerb.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c case_lowerb.c

case_startb.o: case_startb.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c case_startb.c

case_starts.o: case_starts.c case.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c case_starts.c

droproot.o: droproot.c droproot.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c droproot.c

e.o: e.c e.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c e.c

file.o: file.c
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c file.c

filetype.o: filetype.c filetype.h stralloc.h str.h case.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c filetype.c

httpdate.o: httpdate.c httpdate.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c httpdate.c

httpfile.o: httpfile.c stralloc.h pathdecode.h httpdate.h seconds.h \
  percent.h case.h log.h byte.h str.h numtostr.h filetype.h file.h \
  droproot.h alloc.h timeoutwrite.h rangeparser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c httpfile.c

log.o: log.c e.h log.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c log.c

milliseconds.o: milliseconds.c milliseconds.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c milliseconds.c

numtostr.o: numtostr.c numtostr.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c numtostr.c

pathdecode.o: pathdecode.c pathdecode.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c pathdecode.c

percent.o: percent.c percent.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c percent.c

rangeparser.o: rangeparser.c rangeparser.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c rangeparser.c

seconds.o: seconds.c seconds.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c seconds.c

str.o: str.c str.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c str.c

stralloc.o: stralloc.c alloc.h e.h stralloc.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c stralloc.c

timeoutwrite.o: timeoutwrite.c e.h milliseconds.h timeoutwrite.h
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c timeoutwrite.c

utime.o: utime.c
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c utime.c

httpfile: httpfile.o  alloc.o byte_chr.o byte_isequal.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o case_starts.o droproot.o e.o file.o filetype.o httpdate.o log.o milliseconds.o numtostr.o pathdecode.o percent.o rangeparser.o seconds.o str.o stralloc.o timeoutwrite.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o httpfile httpfile.o  alloc.o byte_chr.o byte_isequal.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o case_starts.o droproot.o e.o file.o filetype.o httpdate.o log.o milliseconds.o numtostr.o pathdecode.o percent.o rangeparser.o seconds.o str.o stralloc.o timeoutwrite.o $(LDFLAGS)

utime: utime.o  alloc.o byte_chr.o byte_isequal.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o case_starts.o droproot.o e.o file.o filetype.o httpdate.o log.o milliseconds.o numtostr.o pathdecode.o percent.o rangeparser.o seconds.o str.o stralloc.o timeoutwrite.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o utime utime.o  alloc.o byte_chr.o byte_isequal.o case_diffb.o case_diffs.o case_lowerb.o case_startb.o case_starts.o droproot.o e.o file.o filetype.o httpdate.o log.o milliseconds.o numtostr.o pathdecode.o percent.o rangeparser.o seconds.o str.o stralloc.o timeoutwrite.o $(LDFLAGS)

clean:
	rm -f *.o  httpfile utime

