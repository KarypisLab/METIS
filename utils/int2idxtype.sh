#!/bin/sh

for file in *.[c,h]
  do 
    cat $file | sed 's/ int / idxtype /g' > $file.tmp
    mv $file.tmp $file
    cat $file | sed 's/(int /(idxtype /g' > $file.tmp
    mv $file.tmp $file
    cat $file | sed 's/^int /idxtype /g' > $file.tmp
    mv $file.tmp $file
  done

