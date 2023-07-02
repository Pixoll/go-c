#!/bin/bash

exename=$1
if [[ -z $exename ]]; then
    exename=go
fi

files_common=""
for file in `find ./src/common -type f -name "*.c"`; do
    files_common="$files_common \"$file\"" 
done

files_console=$files_common
for file in `find ./src/console -type f -name "*.c"`; do
    files_console="$files_console \"$file\"" 
done

command_console="gcc -g $files_console -o $exename-console.exe"
echo Running \(console version\): $command_console
eval $command_console

files_sdl=$files_common
for file in `find ./src/sdl -type f -name "*.c"`; do
    files_sdl="$files_sdl \"$file\"" 
done

command_sdl="gcc -g $files_sdl -o $exename-sdl.exe -lSDL2"
echo Running \(SDL version\): $command_sdl
eval $command_sdl
