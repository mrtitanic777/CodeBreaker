@echo off
REM Reusable env for the from-source OpenWatcom v2 release tree.
REM Usage: call owenv.bat   (then run wcc / wcc386 / wlink / wrc)
set "NoDefaultCurrentDirectoryInExePath="
set WATCOM=C:\Users\mrtit\Documents\GitHub\CodeBreaker\experiments\premise-proof\open-watcom-v2\rel
set PATH=%WATCOM%\binnt64;%PATH%
REM h\nt = Win32 headers, h\win = Win16 headers, h = common.
set INCLUDE=%WATCOM%\h;%WATCOM%\h\nt;%WATCOM%\h\win
