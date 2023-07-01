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

rem set files_sdl=!files_common!
rem for /f "tokens=*" %%f in ('dir src\sdl\*.c /s/b') do (
rem     set files_sdl=!files_sdl! "%%f"
rem )

rem set command_sdl=gcc -g !files_sdl! -o "!exename!-sdl.exe"
rem echo Running: %command_sdl%
rem command_sdl%
