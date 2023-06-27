#!/bin/bash

exename=$1
if [[ -z $exename ]]; then
    exename=go
fi

files=""
for file in `find ./src -type f -name "*.c"`; do
    files="$files \"$file\"" 
done

command="gcc -g $files -o $exename.exe"
echo Running: $command
eval $command
