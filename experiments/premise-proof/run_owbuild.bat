@echo off
REM Build OpenWatcom v2 from source, bootstrapped with MSVC (VISUALC).
REM Docs disabled (OWNOWGML=1 / OWDOCBUILD=0) so no DOSBox/WGML dependency.
REM Mirrors the proven probe.bat prologue (no setlocal).
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
REM OW's bootstrap runs freshly-built helpers (wsplice, getversi) by bare name
REM from the current dir; this environment sets NoDefaultCurrentDirectoryInExePath=1
REM which blocks that. Clear it so cwd-relative exec works.
set "NoDefaultCurrentDirectoryInExePath="
set OWROOT=C:\Users\mrtit\Documents\GitHub\CodeBreaker\experiments\premise-proof\open-watcom-v2
set OWTOOLS=VISUALC
set OWOBJDIR=binbuild
set OWDOCBUILD=0
set OWNOWGML=1
set OWDISTRBUILD=0
set DOS4G=QUIET
call "%OWROOT%\cmnvars.bat"
cd /d "%OWROOT%"
echo === OW build start: %DATE% %TIME% ===
call "%OWROOT%\build.bat" build
echo === OW build end (errorlevel %ERRORLEVEL%): %DATE% %TIME% ===
