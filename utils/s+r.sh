#!/bin/sh

echo "Replacing $1 with $2"

for file in *.[c,h]
  do 
    cat $file | sed 's/$1/$2/g' > $file.tmp
    mv $file.tmp $file
  done

