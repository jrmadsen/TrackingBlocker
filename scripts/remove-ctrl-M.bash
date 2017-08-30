#!/bin/bash

available(){
    which $1 >/dev/null 2>&1
}

if available ssed
then
    SED_EXE=`which ssed`
elif available perl
then
    SED_EXE="`which perl` -pi -e" 
else
    SED_EXE=`which sed`
fi

for i in $@
do
    $SED_EXE -i 's///g' $i
done
