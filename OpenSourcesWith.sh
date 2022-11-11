#!/bin/bash

files="*.c *.h *.S"

if [ ! -n "$1" ]; then
program="/usr/share/code/code"
else
program="$1"
fi

shift 

if [ ! -n "$1" ]; then
options="--unity-launch"
else
options="$1"
fi

 

echo "Use $program with options $options on files $files"


$program $options $files &


exit $?
