@echo off
REM Build wthunk.exe (our native C thunk compiler) with the from-source OpenWatcom.
setlocal
set HERE=%~dp0
call "%HERE%owenv.bat"
set INCLUDE=%WATCOM%\h;%WATCOM%\h\nt
cd /d "%HERE%wthunk"
wcl386 -bt=nt -l=nt -q wthunk.c
echo build_wthunk errorlevel=%ERRORLEVEL%
dir wthunk.exe 2>nul | findstr wthunk.exe
endlocal
