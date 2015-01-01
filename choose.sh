
header=$1
shift

for i in $@; do
  cp $i $header.test.c
  ./compile -O $header.test.c >/dev/null 2>&1 || { rm -f $header.test.c; continue; }
  rm -f $header.test.c
  ./load $header.test >/dev/null 2>&1 || { rm -f $header.test.o; continue; }
  rm -f $header.test.o
  ./$header.test > $header.tmp || {  rm -f $header.test $header.tmp; continue; }
  rm -f $header.test
  mv $header.tmp $header
  exit 0
done

echo "#error unsupported $header" > $header
exit 0

