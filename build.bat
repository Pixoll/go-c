@echo off
setlocal EnableDelayedExpansion

set exename=%*
if [!exename!] == [] set exename=go

set files_common=
for /f "tokens=*" %%f in ('dir src\common\*.c /s/b') do (
    set files_common=!files_common! "%%f"
)

set files_console=!files_common!
for /f "tokens=*" %%f in ('dir src\console\*.c /s/b') do (
    set files_console=!files_console! "%%f"
)

set command_console=gcc -g !files_console! -o "!exename!-console.exe"
echo Running: %command_console%
%command_console%

set files_sdl=!files_common!
for /f "tokens=*" %%f in ('dir src\sdl\*.c /s/b') do (
    set files_sdl=!files_sdl! "%%f"
)

set command_sdl=gcc -g !files_sdl! -lSDL2 -o "!exename!-sdl.exe"
echo Running: %command_sdl%
command_sdl%
