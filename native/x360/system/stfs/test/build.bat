@echo off
rem Builds and runs the STFS library's tests (runtime/stfs/test) with clang-cl from Visual Studio 2026, set up the way
rem the dashboard's build.bat does. Build output and test packages go to %TEMP% (nothing is written into Shared\).
rem   build.bat            build, self test, then read / verify / compare (with tools\scripts\stfs_extract.py) the
rem                        system update packages in ..\..\..\..\Xbox 360 Dashboard\SystemUpdate\$SystemUpdate\ when present
rem   build.bat <pkg>...   the same with these packages instead (read only; never copied)
setlocal
cd /d "%~dp0"
set "BUILD=%TEMP%\stfs_test_build"
set "WORK=%TEMP%\stfs_test_work"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto :no_vs
set "VSDIR="
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -prerelease -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSDIR=%%i"
if not defined VSDIR goto :no_vs
set "CLANGCL=%VSDIR%\VC\Tools\Llvm\x64\bin\clang-cl.exe"
if not exist "%CLANGCL%" goto :no_vs
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1 || exit /b 1

if exist "%BUILD%\CMakeCache.txt" goto :build
cmake -S . -B "%BUILD%" -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_CXX_COMPILER=%CLANGCL%" || exit /b 1
:build
cmake --build "%BUILD%" || exit /b 1

set "PACKAGES=%*"
set "HAVE=%~1"
if defined HAVE goto :run
set "SU=..\..\..\..\Xbox 360 Dashboard\SystemUpdate\$SystemUpdate"
if not exist "%SU%\su20076000_00000000" goto :run
set "HAVE=1"
set PACKAGES="%SU%\su20076000_00000000" "%SU%\FFFE07DF00000001" "%SU%\FFFE07DF00000002" "%SU%\FFFE07DF00000006" "%SU%\FFFE07DF00000008"

:run
"%BUILD%\stfs_test.exe" selftest "%WORK%" %PACKAGES% || exit /b 1
if not defined HAVE goto :done
python -I compare.py "%BUILD%\stfs_test.exe" %PACKAGES% || exit /b 1
:done
echo STFS tests passed.
exit /b 0

:no_vs
echo Needs Visual Studio 2026 with "Desktop development with C++" (incl. the Clang compiler and CMake tools).
exit /b 1
