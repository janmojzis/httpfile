#!/bin/bash

top="`pwd`"
build="`pwd`/build"

rm -rf "${build}"
#mkdir -p "${build}"

#cp *.c *.h "${build}"
(
  #cd "${build}"
  (
    echo "CC=cc"
    echo "CFLAGS+=-W -Wall -Os -fPIC -fwrapv -Wall"
    #echo "LDFLAGS+=-L../BearSSL/build -lbearssl -static"
    echo "LDFLAGS+="
    echo 

    binaries=""
    objects=""
    for file in `ls *.c`; do
      if grep '^int main(' "${file}" >/dev/null; then
        x=`echo "${file}" | sed 's/\.c$//'`
        binaries="${binaries} ${x}"
      else
        x=`echo "${file}" | sed 's/\.c$/.o/'`
        objects="${objects} ${x}"
      fi
    done

    echo "all: ${binaries}"
    echo 

    for file in `ls *.c`; do
      (
        gcc -I../BearSSL/inc -MM "${file}"
        start=`echo "${file}" | cut -b1-7`
        if [ x"`echo "${file}" | cut -b1-7`" = xcrypto_ ]; then
          opt="-O3"
        else
          opt=""
        fi
        echo -e "\t\$(CC) \$(CFLAGS) \$(CPPFLAGS) ${opt} -c ${file}"
        echo 
      )
    done

    for file in `ls *.c`; do
      if grep '^int main(' "${file}" >/dev/null; then
        x=`echo "${file}" | sed 's/\.c$//'`
        echo "${x}: ${x}.o ${objects}"
        echo -e "\t\$(CC) \$(CFLAGS) \$(CPPFLAGS) -o ${x} ${x}.o ${objects} \$(LDFLAGS)"
        echo 
      fi
    done

    echo "rts.out: ${binaries} rts.tests rts.exp"
    echo -e "\tsh rts.tests > rts.out"
    echo

    echo "test: rts.exp rts.out"
    echo -e "\tdiff rts.exp rts.out"
    echo

    echo "clean:"
    echo -e "\trm -f *.o ${binaries} rts.out"
    echo 

  ) > Makefile
)
