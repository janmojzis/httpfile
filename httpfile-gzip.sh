
PATH="HTTPFILE/bin:${PATH}";
export PATH

usage="httpfile-gzip: usage: httpfile-gzip /public"

if [ x"$1" = x ]; then
  echo "${usage}"
  exit 100
fi

if [ ! -d "$1" ]; then
  echo "${usage}"
  exit 100
fi

cd "$1"

if [ ! -d plain ]; then
  echo "${usage}"
  exit 111
fi

httpfile-setlock .lock sh -ce '
rm -rf gzip.tmp
find plain |\
while read file; do
  f=`echo "${file}" | sed "s,^plain,gzip.tmp,"`
  fold=`echo "${file}" | sed "s,^plain,gzip,"`
  if [ -h "${file}" ]; then
    cp -pr "${file}" "${f}"
    continue
  fi
  if [ -d "${file}" ]; then
    mkdir "${f}"
    continue
  fi
  if [ -f "${file}" ]; then
    ext=`echo "${f}" | awk '"'"'BEGIN{FS="."}{print \$NF}'"'"'`
    if [ x"$ext" != xtxt ]; then
      if [ x"$ext" != xhtml ]; then
        if [ x"$ext" != xcss ]; then
          if [ x"$ext" != xjs ]; then
            continue
          fi
        fi
      fi
    fi

    mtimeplain=`httpfile-getmtime "${file}"`
    mtimegzipold=`httpfile-getmtime "${fold}" 2>/dev/null || :`
    if [ x"${mtimeplain}" = x"${mtimegzipold}" ]; then
      ln "${fold}" "${f}"
      continue
    fi

    gzip -9 < "${file}" >"${f}"
    httpfile-setmtime "${f}" "${mtimeplain}"
    continue
  fi
done
if [ -d gzip ]; then
  mv gzip gzip.old
fi
mv gzip.tmp gzip
rm -rf gzip.old
'
