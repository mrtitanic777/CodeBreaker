@echo off
rem run.cmd -- runs test.exe on cases.txt (written by expected.py), writes actual.txt
rem TEMP is the tmp\ folder beside this file (as for expected.py): the assemblers' work folders (emptied first).
pushd "%~dp0"
set CB_CHECKOUT_ROOT=C:\Users\mrtit\Documents\GitHub\CodeBreaker
set CB_CORPUS_ROOT=C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files
set CB_CORPUS_NAME=
set TEMP=%~dp0tmp
set TMP=%~dp0tmp
if exist tmp rmdir /s /q tmp
mkdir tmp
if exist runs rmdir /s /q runs
mkdir runs
mkdir state_cpp
for /f "delims=" %%i in ('python -c "import unicorn,os;print(os.path.join(os.path.dirname(unicorn.__file__),'lib','unicorn.dll'))"') do set CB_UNICORN_DLL=%%i
"%~dp0test.exe" cases.txt capstone.txt > actual.txt
popd
