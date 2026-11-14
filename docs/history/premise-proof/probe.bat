@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat" >nul 2>&1
set OWROOT=C:\Users\mrtit\Documents\GitHub\CodeBreaker\experiments\premise-proof\open-watcom-v2
set OWTOOLS=VISUALC
set OWNOWGML=1
set OWDOCBUILD=0
call "%OWROOT%\cmnvars.bat" >nul 2>&1
echo --- probe ---
echo CWD_AFTER_CMNVARS=%CD%
echo CL location:
where cl
cd /d "%OWROOT%"
echo AFTER_CD=%CD%
if exist build.bat (echo BUILD_BAT_FOUND) else (echo BUILD_BAT_MISSING)
