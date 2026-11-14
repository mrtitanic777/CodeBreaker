@echo off
REM Build wdef.exe (our MS .def -> wlink directive translator) with OpenWatcom.
setlocal
set HERE=%~dp0
call "%HERE%owenv.bat"
set INCLUDE=%WATCOM%\h;%WATCOM%\h\nt
cd /d "%HERE%wdef"
wcl386 -bt=nt -l=nt -q wdef.c
echo build_wdef errorlevel=%ERRORLEVEL%
dir wdef.exe 2>nul | findstr wdef.exe
endlocal
