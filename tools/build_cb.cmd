@echo off
rem build_cb.cmd [asan] [-- cb_tests args] -- build src (docs/CPP_DESIGN.md) and run its tests.
rem
rem     tools\build_cb.cmd                       Release build in build, run every test
rem     tools\build_cb.cmd area exact            only area exact: its library, what it links, its tests (fast)
rem     tools\build_cb.cmd asan                  /fsanitize=address build in build-asan, run every test
rem     tools\build_cb.cmd -- --filter core.io   run only the tests whose name contains core.io
rem
rem Builds use half the cores (CB_BUILD_JOBS overrides): a build on every core starves everything else on the machine.
rem Needs MSVC (Visual Studio 2026), CMake and Ninja; the vcvars environment is loaded if it is not already.
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
set MODE=release
set B=%ROOT%\build
set EXTRA=
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
if "%1"=="asan" (
  set MODE=asan
  set B=%ROOT%\build-asan
  set EXTRA=-DCB_ASAN=ON
  shift
)
if "%1"=="--" shift
if not defined VCINSTALLDIR set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\src" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl %EXTRA% || exit /b 1
rem tools\build_cb.cmd area NAME [args] -- only one area's library, what it links, and its own tests, then run them
if "%1"=="area" (
  cmake --build "%B%" --target cb_tests_%2 -j %JOBS% || exit /b 1
  "%B%\cb_tests_%2.exe" %3 %4 %5 %6 %7 %8 %9
  exit /b %ERRORLEVEL%
)
cmake --build "%B%" -j %JOBS% || exit /b 1
"%B%\cb_tests.exe" %1 %2 %3 %4 %5 %6 %7 %8 %9
exit /b %ERRORLEVEL%
