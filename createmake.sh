#!/bin/bash

echo "default: it"
echo

for file in `ls -1 *.c`;do
 obj=`echo "$file" | sed s/c$/o/`
 echo "$obj: \\"
 for header in `./getheader.sh $file | sort | uniq`; do
   echo "$header \\"
 done
 echo "$file \\"
 echo "compile"
 echo "	./compile $file"
 echo
done

c=8

echo "library.a: \\"
echo -n "makelib "

ls *.c \
| sort \
| while read file
do
  if grep "main(" "${file}" >/dev/null; then
    continue
  fi
  ofile=`echo "${file}" | sed "s/\.c$/\.o/"`
  l=`echo "${file}" | wc -c`
  c=$((${c}+${l}))
  echo -n "${ofile} "
  if [ "${c}" -gt 40 ]; then
    echo "\\"
    c=0
  fi
done
echo

c=20
echo -n -e "\t./makelib library.a "

ls *.c \
| sort \
| while read file
do
  if grep "main(" "${file}" >/dev/null; then
    continue
  fi
  ofile=`echo "${file}" | sed "s/\.c$/\.o/"`
  l=`echo "${file}" | wc -c`
  c=$((${c}+${l}))
  echo -n "${ofile} "
  if [ "${c}" -gt 40 ]; then
    echo "\\"
    echo -n -e "\t"
    c=0
  fi
done

echo
echo

cat Makefile.templ


