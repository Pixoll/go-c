@echo off
setlocal EnableDelayedExpansion

set exename=%*
if [!exename!] == [] set exename=go

cd src
set files=
for /f "tokens=*" %%f in ('dir /s /b *.c') do (
    set files=!files! "%%f"
)

cd ..
set command=gcc -g !files! -o "!exename!.exe"
echo Running: %command%
%command%
