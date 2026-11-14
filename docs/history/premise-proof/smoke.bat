@echo off
REM Smoke-test the from-source OpenWatcom v2: compile + link a Win16 NE.
set "NoDefaultCurrentDirectoryInExePath="
set WATCOM=C:\Users\mrtit\Documents\GitHub\CodeBreaker\experiments\premise-proof\open-watcom-v2\rel
set PATH=%WATCOM%\binnt64;%PATH%
set INCLUDE=%WATCOM%\h;%WATCOM%\h\win
cd /d C:\Users\mrtit\Documents\GitHub\CodeBreaker\experiments\premise-proof\smoke
echo === wcc version ===
wcc -? 2>&1 | findstr /i "Open Watcom C"
echo === compile (16-bit, medium model, windows target) ===
wcc -bt=windows -mc -zW hello.c
echo compile errorlevel=%ERRORLEVEL%
echo === link to Win16 NE ===
wlink system windows name hello.exe file hello.obj
echo link errorlevel=%ERRORLEVEL%
echo === output ===
dir hello.exe
