#!/bin/sh

license_tmp=`mktemp`
echo "/*" > $license_tmp
cat LICENSE.txt >> $license_tmp
echo "*/" >> $license_tmp
echo "" >> $license_tmp

file_tmp=`mktemp`
#files="src/hex_hav/Chrono.hpp"
files=`find src -name "*pp"`
for file in $files ; do
    cat $license_tmp $file > $file_tmp
    cp $file_tmp $file
done

