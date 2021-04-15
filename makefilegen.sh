#!/bin/bash

(
  (
    echo "CC?=cc"
    echo "CFLAGS+=-W -Wall -Os -fPIC -fwrapv -Wall"
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
        echo -e "\t\$(CC) \$(CFLAGS) \$(CPPFLAGS) -c ${file}"
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

    echo "rts.out: ${binaries} rts.tests"
    echo -e "\tsh rts.tests > rts.out"
    echo

    echo "rts: rts.exp rts.out"
    echo -e "\tdiff rts.exp rts.out"
    echo

    echo "clean:"
    echo -e "\trm -f *.o ${binaries} rts.out"
    echo 

  ) > Makefile
)
