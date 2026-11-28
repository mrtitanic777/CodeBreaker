@echo off
rem tools\sdk\reference.cmd -- writes docs\sdk\reference from the comments of include\cb (src/sdk/reference.cpp; the test
rem sdk.reference.is_current fails when the pages are not what this writes). Builds cb_sdk_reference in build\ first.
setlocal
set ROOT=%~dp0..\..
for %%I in ("%ROOT%") do set ROOT=%%~fI
if not defined VCINSTALLDIR set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%ROOT%\build\build.ninja" cmake -S "%ROOT%\src" -B "%ROOT%\build" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl || exit /b 1
cmake --build "%ROOT%\build" --target cb_sdk_reference || exit /b 1
"%ROOT%\build\cb_sdk_reference.exe" "%ROOT%"
