@echo off
REM Rebuild the 16-bit LZEXPAND.DLL (NE) from recovered source with OpenWatcom's
REM wcc (medium model, Win16) -- NO Microsoft tools. The codec LZ32 thunks down to.
setlocal
set HERE=%~dp0
set SRC=C:\Users\mrtit\Documents\GitHub\Windows95\base\lzexpand
set BLD=%HERE%lzexpand-build
call "%HERE%owenv.bat"
REM 16-bit build: Win16 headers only (h\win), NOT the Win32 h\nt windows.h
set INCLUDE=%WATCOM%\h;%WATCOM%\h\win
if not exist "%BLD%" mkdir "%BLD%"
copy /y "%SRC%\*.c" "%BLD%\" >nul
copy /y "%SRC%\*.h" "%BLD%\" >nul
copy /y "%SRC%\lzexpand.def" "%BLD%\" >nul
copy /y "%HERE%lzexpand.lnk" "%BLD%\" >nul
cd /d "%BLD%"
set MODS=lzalloc lzread lzwep lzcopy lzstate lzio lzcopyf lzhdr lzdecomp lzname lzdata
echo === compile (wcc -bt=windows -mm) ===
set FAIL=0
for %%m in (%MODS%) do (
  wcc -bt=windows -mm -zW -q %%m.c || (echo   COMPILE FAIL: %%m & set FAIL=1)
)
if "%FAIL%"=="1" (echo COMPILE STAGE HAD FAILURES & goto done)
echo   all modules compiled OK
echo === exports from lzexpand.def via wdef ===
if not exist "%HERE%wdef\wdef.exe" call "%HERE%build_wdef.bat" >nul
"%HERE%wdef\wdef.exe" "%SRC%\lzexpand.def" > lzexpand.exports.lnk
echo === link (wlink system windows_dll) ===
wlink @lzexpand.lnk @lzexpand.exports.lnk
echo LINK_EL=%ERRORLEVEL%
dir lzexpand.dll 2>nul | findstr lzexpand.dll
:done
endlocal
