@echo off
rem build_cb.cmd [asan] [-- cb_tests args] -- build native/cb (docs/CPP_DESIGN.md) and run its tests.
rem
rem     tools\build_cb.cmd                       Release build in state\cxx\cb, run every test
rem     tools\build_cb.cmd asan                  /fsanitize=address build in state\cxx\cb-asan, run every test
rem     tools\build_cb.cmd -- --filter core.io   run only the tests whose name contains core.io
rem
rem Needs MSVC (Visual Studio 2026), CMake and Ninja; the vcvars environment is loaded if it is not already.
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
set MODE=release
set B=%ROOT%\state\cxx\cb
set EXTRA=
if "%1"=="asan" (
  set MODE=asan
  set B=%ROOT%\state\cxx\cb-asan
  set EXTRA=-DCB_ASAN=ON
  shift
)
if "%1"=="--" shift
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\native" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release %EXTRA% || exit /b 1
cmake --build "%B%" || exit /b 1
"%B%\bin\cb_tests.exe" %1 %2 %3 %4 %5 %6 %7 %8 %9
exit /b %ERRORLEVEL%
