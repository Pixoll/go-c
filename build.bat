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
echo Running (console version): %command_console%
%command_console%

set files_sdl=!files_common!
for /f "tokens=*" %%f in ('dir src\sdl\*.c /s/b') do (
    set files_sdl=!files_sdl! "%%f"
)

set command_sdl=gcc !files_sdl! -I.\WIN_SDL2\include -L.\WIN_SDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o "!exename!-sdl.exe"
echo Running (SDL version): %command_sdl%
%command_sdl%
