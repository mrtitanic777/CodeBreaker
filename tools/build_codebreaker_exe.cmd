@echo off
rem build_codebreaker_exe.cmd -- THE program: codebreaker.exe, every command of CodeBreaker on src, no Python behind it
rem (with no command it is the Windows app: app/window_win.cpp).
rem
rem     tools\build_codebreaker_exe.cmd          builds target `codebreaker` in build\ (the tree tools\build_cb.cmd builds, so the
rem                                              objects are shared): build\codebreaker.exe, with the DLLs it loads and the
rem                                              programs it starts (decompiler.exe, pcodecheck.exe) beside it -- the path
rem                                              .mcp.json and tools\gate.cmd use
rem
rem Needs MSVC, CMake, Ninja. CB_BUILD_JOBS limits the build (default every core). A running codebreaker.exe (the background service,
rem an MCP server) cannot be overwritten but can be renamed: the build moves it aside before it links (tools\replace_running.cmd).
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
set B=%ROOT%\build
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set JOBS=%NUMBER_OF_PROCESSORS%
if not defined VCINSTALLDIR set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\src" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl || exit /b 1
cmake --build "%B%" --target codebreaker decompiler pcodecheck -j %JOBS% || exit /b 1
echo built %B%\codebreaker.exe
exit /b 0
