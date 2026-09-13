@echo off
REM Assemble the built OpenWatcom binaries into a usable release tree (OWROOT\rel):
REM binnt/binnt64 host tools + h headers + lib286/lib386 target libraries.
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
set "NoDefaultCurrentDirectoryInExePath="
set OWROOT=C:\Users\mrtit\Documents\GitHub\CodeBreaker\experiments\premise-proof\open-watcom-v2
set OWTOOLS=VISUALC
set OWOBJDIR=binbuild
set OWDOCBUILD=0
set OWNOWGML=1
set OWDISTRBUILD=0
set OWRELROOT=%OWROOT%\rel
set DOS4G=QUIET
call "%OWROOT%\cmnvars.bat"
cd /d "%OWROOT%\bld"
echo === OW cprel start: %DATE% %TIME% ===
builder cprel
echo === OW cprel end (errorlevel %ERRORLEVEL%): %DATE% %TIME% ===
