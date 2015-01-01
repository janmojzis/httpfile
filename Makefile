default: it

alloc.o: \
alloc.h \
e.h \
alloc.c \
compile
	./compile alloc.c

alloc_re.o: \
alloc.h \
byte.h \
alloc_re.c \
compile
	./compile alloc_re.c

auth.o: \
auth.h \
byte.h \
cdb.h \
e.h \
gen_alloc.h \
open.h \
pathdecode.h \
percent.h \
stralloc.h \
str.h \
uint32.h \
auth.c \
compile
	./compile auth.c

auto-str.o: \
buffer.h \
auto-str.c \
compile
	./compile auto-str.c

base64.o: \
base64.h \
e.h \
gen_alloc.h \
stralloc.h \
base64.c \
compile
	./compile base64.c

buffer_0.o: \
buffer.h \
buffer_0.c \
compile
	./compile buffer_0.c

buffer_1.o: \
buffer.h \
buffer_1.c \
compile
	./compile buffer_1.c

buffer_2.o: \
buffer.h \
buffer_2.c \
compile
	./compile buffer_2.c

buffer.o: \
buffer.h \
buffer.c \
compile
	./compile buffer.c

buffer_copy.o: \
buffer.h \
buffer_copy.c \
compile
	./compile buffer_copy.c

buffer_get.o: \
buffer.h \
byte.h \
e.h \
buffer_get.c \
compile
	./compile buffer_get.c

buffer_put.o: \
buffer.h \
byte.h \
e.h \
str.h \
buffer_put.c \
compile
	./compile buffer_put.c

buffer_read.o: \
buffer.h \
buffer_read.c \
compile
	./compile buffer_read.c

buffer_write.o: \
buffer.h \
buffer_write.c \
compile
	./compile buffer_write.c

byte_chr.o: \
byte.h \
byte_chr.c \
compile
	./compile byte_chr.c

byte_copy.o: \
byte.h \
byte_copy.c \
compile
	./compile byte_copy.c

byte_isequal.o: \
byte.h \
byte_isequal.c \
compile
	./compile byte_isequal.c

case_diffb.o: \
case.h \
case_diffb.c \
compile
	./compile case_diffb.c

case_diffs.o: \
case.h \
case_diffs.c \
compile
	./compile case_diffs.c

case_lowerb.o: \
case.h \
case_lowerb.c \
compile
	./compile case_lowerb.c

case_startb.o: \
case.h \
case_startb.c \
compile
	./compile case_startb.c

case_starts.o: \
case.h \
case_starts.c \
compile
	./compile case_starts.c

cdb.o: \
byte.h \
cdb.h \
e.h \
uint32.h \
cdb.c \
compile
	./compile cdb.c

cdb_hash.o: \
cdb.h \
uint32.h \
cdb_hash.c \
compile
	./compile cdb_hash.c

cdb_make.o: \
alloc.h \
buffer.h \
cdb.h \
cdb_make.h \
e.h \
uint32.h \
cdb_make.c \
compile
	./compile cdb_make.c

droproot.o: \
env.h \
prot.h \
scan.h \
strerr.h \
droproot.c \
compile
	./compile droproot.c

e.o: \
e.h \
e.c \
compile
	./compile e.c

env.o: \
env.h \
str.h \
env.c \
compile
	./compile env.c

e_temp.o: \
e.h \
e_temp.c \
compile
	./compile e_temp.c

file.o: \
buffer.h \
byte.h \
e.h \
env.h \
file.h \
open.h \
strerr.h \
str.h \
file.c \
compile
	./compile file.c

filetype.o: \
case.h \
filetype.h \
gen_alloc.h \
stralloc.h \
str.h \
filetype.c \
compile
	./compile filetype.c

fmt_longlong.o: \
fmt.h \
fmt_longlong.c \
compile
	./compile fmt_longlong.c

fmt_ulonglong.o: \
fmt.h \
fmt_ulonglong.c \
compile
	./compile fmt_ulonglong.c

fmt_xlonglong.o: \
fmt.h \
fmt_xlonglong.c \
compile
	./compile fmt_xlonglong.c

getln2.o: \
buffer.h \
byte.h \
gen_alloc.h \
getln.h \
stralloc.h \
getln2.c \
compile
	./compile getln2.c

getln.o: \
buffer.h \
byte.h \
gen_alloc.h \
getln.h \
stralloc.h \
getln.c \
compile
	./compile getln.c

hier.o: \
auto_home.h \
hier.c \
compile
	./compile hier.c

httpdate.o: \
gen_alloc.h \
httpdate.h \
stralloc.h \
httpdate.c \
compile
	./compile httpdate.c

httpfile-access.o: \
base64.h \
buffer.h \
byte.h \
case.h \
cdb_make.h \
fmt.h \
gen_alloc.h \
getln.h \
open.h \
pathdecode.h \
percent.h \
stralloc.h \
strerr.h \
uint32.h \
httpfile-access.c \
compile
	./compile httpfile-access.c

httpfile-conf.o: \
auto_home.h \
buffer.h \
hasdevtcp.h \
open.h \
strerr.h \
httpfile-conf.c \
compile
	./compile httpfile-conf.c

httpfile-getmtime.o: \
buffer.h \
fmt.h \
strerr.h \
httpfile-getmtime.c \
compile
	./compile httpfile-getmtime.c

httpfile-httpd.o: \
auth.h \
buffer.h \
byte.h \
case.h \
droproot.h \
e.h \
env.h \
file.h \
filetype.h \
fmt.h \
gen_alloc.h \
getln.h \
httpdate.h \
pathdecode.h \
percent.h \
scan.h \
seconds.h \
sig.h \
stralloc.h \
strerr.h \
str.h \
timeoutread.h \
timeoutwrite.h \
httpfile-httpd.c \
compile
	./compile httpfile-httpd.c

httpfile-setlock.o: \
open.h \
strerr.h \
httpfile-setlock.c \
compile
	./compile httpfile-setlock.c

httpfile-setmtime.o: \
scan.h \
strerr.h \
httpfile-setmtime.c \
compile
	./compile httpfile-setmtime.c

install.o: \
buffer.h \
e.h \
open.h \
strerr.h \
install.c \
compile
	./compile install.c

instcheck.o: \
e.h \
strerr.h \
instcheck.c \
compile
	./compile instcheck.c

milliseconds.o: \
milliseconds.h \
milliseconds.c \
compile
	./compile milliseconds.c

open_lock.o: \
open.h \
open_lock.c \
compile
	./compile open_lock.c

open_read.o: \
open.h \
open_read.c \
compile
	./compile open_read.c

open_trunc.o: \
open.h \
open_trunc.c \
compile
	./compile open_trunc.c

pathdecode.o: \
gen_alloc.h \
pathdecode.h \
stralloc.h \
pathdecode.c \
compile
	./compile pathdecode.c

percent.o: \
gen_alloc.h \
percent.h \
stralloc.h \
percent.c \
compile
	./compile percent.c

prot.o: \
hasshsgr.h \
prot.h \
prot.c \
compile
	./compile prot.c

scan_longlong.o: \
scan.h \
scan_longlong.c \
compile
	./compile scan_longlong.c

scan_plusminus.o: \
scan.h \
scan_plusminus.c \
compile
	./compile scan_plusminus.c

scan_ulonglong.o: \
scan.h \
scan_ulonglong.c \
compile
	./compile scan_ulonglong.c

seconds.o: \
seconds.h \
seconds.c \
compile
	./compile seconds.c

sig_block.o: \
sig.h \
sig_block.c \
compile
	./compile sig_block.c

sig_catch.o: \
sig.h \
sig_catch.c \
compile
	./compile sig_catch.c

stralloc_append.o: \
alloc.h \
gen_allocdefs.h \
gen_alloc.h \
stralloc.h \
stralloc_append.c \
compile
	./compile stralloc_append.c

stralloc_catb.o: \
byte.h \
gen_alloc.h \
stralloc.h \
stralloc_catb.c \
compile
	./compile stralloc_catb.c

stralloc_cat.o: \
byte.h \
gen_alloc.h \
stralloc.h \
stralloc_cat.c \
compile
	./compile stralloc_cat.c

stralloc_cats.o: \
byte.h \
gen_alloc.h \
stralloc.h \
str.h \
stralloc_cats.c \
compile
	./compile stralloc_cats.c

stralloc_copyb.o: \
byte.h \
gen_alloc.h \
stralloc.h \
stralloc_copyb.c \
compile
	./compile stralloc_copyb.c

stralloc_copy.o: \
byte.h \
gen_alloc.h \
stralloc.h \
stralloc_copy.c \
compile
	./compile stralloc_copy.c

stralloc_copys.o: \
byte.h \
gen_alloc.h \
stralloc.h \
str.h \
stralloc_copys.c \
compile
	./compile stralloc_copys.c

stralloc_num.o: \
gen_alloc.h \
stralloc.h \
stralloc_num.c \
compile
	./compile stralloc_num.c

stralloc_ready.o: \
alloc.h \
gen_allocdefs.h \
gen_alloc.h \
stralloc.h \
stralloc_ready.c \
compile
	./compile stralloc_ready.c

str_chr.o: \
str.h \
str_chr.c \
compile
	./compile str_chr.c

str_diff.o: \
str.h \
str_diff.c \
compile
	./compile str_diff.c

strerr_die.o: \
buffer.h \
strerr.h \
strerr_die.c \
compile
	./compile strerr_die.c

strerr_sys.o: \
e.h \
strerr.h \
strerr_sys.c \
compile
	./compile strerr_sys.c

str_len.o: \
str.h \
str_len.c \
compile
	./compile str_len.c

str_rchr.o: \
str.h \
str_rchr.c \
compile
	./compile str_rchr.c

str_start.o: \
str.h \
str_start.c \
compile
	./compile str_start.c

timeoutread.o: \
e.h \
milliseconds.h \
timeoutread.h \
timeoutread.c \
compile
	./compile timeoutread.c

timeoutwrite.o: \
e.h \
milliseconds.h \
timeoutwrite.h \
timeoutwrite.c \
compile
	./compile timeoutwrite.c

trylib.o: \
trylib.c \
compile
	./compile trylib.c

uint32_pack.o: \
uint32.h \
uint32_pack.c \
compile
	./compile uint32_pack.c

uint32_unpack.o: \
uint32.h \
uint32_unpack.c \
compile
	./compile uint32_unpack.c

library.a: \
makelib alloc.o alloc_re.o auth.o base64.o \
buffer_0.o buffer_1.o buffer_2.o buffer.o \
buffer_copy.o buffer_get.o buffer_put.o buffer_read.o \
buffer_write.o byte_chr.o byte_copy.o byte_isequal.o \
case_diffb.o case_diffs.o case_lowerb.o case_startb.o \
case_starts.o cdb.o cdb_hash.o cdb_make.o \
droproot.o e.o env.o e_temp.o file.o filetype.o \
fmt_longlong.o fmt_ulonglong.o fmt_xlonglong.o \
getln2.o getln.o hier.o httpdate.o milliseconds.o \
open_lock.o open_read.o open_trunc.o pathdecode.o \
percent.o prot.o scan_longlong.o scan_plusminus.o \
scan_ulonglong.o seconds.o sig_block.o sig_catch.o \
stralloc_append.o stralloc_catb.o stralloc_cat.o \
stralloc_cats.o stralloc_copyb.o stralloc_copy.o \
stralloc_copys.o stralloc_num.o stralloc_ready.o \
str_chr.o str_diff.o strerr_die.o strerr_sys.o \
str_len.o str_rchr.o str_start.o timeoutread.o \
timeoutwrite.o uint32_pack.o uint32_unpack.o \

	./makelib library.a alloc.o alloc_re.o auth.o \
	base64.o buffer_0.o buffer_1.o buffer_2.o \
	buffer.o buffer_copy.o buffer_get.o buffer_put.o \
	buffer_read.o buffer_write.o byte_chr.o byte_copy.o \
	byte_isequal.o case_diffb.o case_diffs.o \
	case_lowerb.o case_startb.o case_starts.o \
	cdb.o cdb_hash.o cdb_make.o droproot.o e.o \
	env.o e_temp.o file.o filetype.o fmt_longlong.o \
	fmt_ulonglong.o fmt_xlonglong.o getln2.o \
	getln.o hier.o httpdate.o milliseconds.o \
	open_lock.o open_read.o open_trunc.o pathdecode.o \
	percent.o prot.o scan_longlong.o scan_plusminus.o \
	scan_ulonglong.o seconds.o sig_block.o sig_catch.o \
	stralloc_append.o stralloc_catb.o stralloc_cat.o \
	stralloc_cats.o stralloc_copyb.o stralloc_copy.o \
	stralloc_copys.o stralloc_num.o stralloc_ready.o \
	str_chr.o str_diff.o strerr_die.o strerr_sys.o \
	str_len.o str_rchr.o str_start.o timeoutread.o \
	timeoutwrite.o uint32_pack.o uint32_unpack.o \
	

it:  httpfile-httpd rts rts.out httpfile-conf httpfile-gzip httpfile-access httpfile-setlock httpfile-getmtime httpfile-setmtime

load: \
warn-auto.sh conf-ld
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo exec "`head -1 conf-ld`" \
	'-o "$$main" "$$main".o $${1+"$$@"}' \
	) > load
	chmod 755 load

compile: \
warn-auto.sh conf-cc
	( cat warn-auto.sh; \
	echo exec "`head -1 conf-cc`" '-c $${1+"$$@"}' \
	) > compile
	chmod 755 compile

choose: \
warn-auto.sh choose.sh compile load
	cat warn-auto.sh choose.sh \
	> choose
	chmod 755 choose

makelib: \
warn-auto.sh
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo 'rm -f "$$main"'; \
	echo 'ar cr "$$main" $${1+"$$@"}'; \
	case "`uname -s | tr '/:[A-Z]' '..[a-z]'`" in \
	sunos-5.*) ;; \
	unix_sv*) ;; \
	irix64-*) ;; \
	irix-*) ;; \
	dgux-*) ;; \
	hp-ux-*) ;; \
	sco*) ;; \
	*) echo 'ranlib "$$main"' ;; \
	esac \
	) > makelib
	chmod 755 makelib


socket.lib: \
trylib.c compile load
	( ( ./compile trylib.c && \
	./load trylib -lsocket -lnsl ) >/dev/null 2>&1 \
	&& echo -lsocket -lnsl || exit 0 ) > socket.lib
	rm -f trylib.o trylib


hasdevtcp.h: \
hasdevtcp.h1 hasdevtcp.h2
	(case "`uname -s | tr '/:[A-Z]' '..[a-z]'`" in \
	sunos-5.*) cat hasdevtcp.h2 ;; \
	*) cat hasdevtcp.h1 ;; \
	esac \
	) > hasdevtcp.h

uint32.h: \
choose tryuint.h uint32.h-int uint32.h-long uint32.h-attr
	./choose uint32.h uint32.h-int uint32.h-long uint32.h-attr

hasshsgr.h: \
choose hasshsgr.h-yes default.h-no
	./choose hasshsgr.h hasshsgr.h-yes default.h-no

clean:
	rm -f load compile makelib choose *.o *.a
	rm -f socket.lib auto_home.c hasdevtcp.h uint32.h hasshsgr.h
	rm -f httpfile-httpd httpfile-setmtime rts rts.out auto-str install instcheck httpfile-gzip httpfile-access httpfile-setlock httpfile-getmtime
	rm -rf access.cdb plain gzip

setup: \
it install
	./install

check: \
it instcheck
	./instcheck


httpfile-httpd: \
load httpfile-httpd.o library.a socket.lib
	./load httpfile-httpd library.a `cat socket.lib`

httpfile-setmtime: \
load httpfile-setmtime.o library.a
	./load httpfile-setmtime library.a

rts: \
warn-auto.sh rts.sh
	cat warn-auto.sh rts.sh > rts
	chmod 755 rts

rts.out: \
rts rts.tests httpfile-httpd httpfile-access httpfile-setmtime rts.access.txt rts.foo rts.foo.gz
	./rts > rts.out

auto-str: \
load auto-str.o library.a
	./load auto-str library.a

auto_home.c: \
auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

auto_home.o: \
compile auto_home.c
	./compile auto_home.c

httpfile-conf: \
load httpfile-conf.o auto_home.o library.a
	./load httpfile-conf auto_home.o library.a

install: \
load install.o auto_home.o library.a
	./load install auto_home.o library.a
	./install

instcheck: \
load instcheck.o auto_home.o library.a
	./load instcheck auto_home.o library.a

httpfile-gzip: \
warn-auto.sh httpfile-gzip.sh conf-home
	cat warn-auto.sh httpfile-gzip.sh | sed s}HTTPFILE}"`head -1 conf-home`"}g  > httpfile-gzip
	chmod 755 httpfile-gzip

httpfile-access: \
load httpfile-access.o library.a
	./load httpfile-access library.a

httpfile-setlock: \
load httpfile-setlock.o library.a
	./load httpfile-setlock library.a

httpfile-getmtime: \
load httpfile-getmtime.o library.a
	./load httpfile-getmtime library.a
