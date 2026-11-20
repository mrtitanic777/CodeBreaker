@echo off
rem build_codebreaker_exe.cmd [OUT] -- THE program: codebreaker.exe, every command of CodeBreaker on native/cb, no Python behind it
rem (with no command it is the Windows app: app/window_win.cpp).
rem
rem     tools\build_codebreaker_exe.cmd          builds target `codebreaker` in state\cxx\cb (the tree tools\build_cb.cmd builds, so the
rem                                              objects are shared) and installs it, with the DLLs it loads, as
rem                                              state\cxx\codebreaker\build\codebreaker.exe -- the path .mcp.json and
rem                                              tools\gate.cmd use
rem     tools\build_codebreaker_exe.cmd OUT      installs into OUT instead
rem
rem Needs MSVC, CMake, Ninja. CB_BUILD_JOBS limits the build (default half the cores). A running codebreaker.exe (the background service, an
rem MCP server) cannot be overwritten but can be renamed: it is moved aside and the new one written; old ones are deleted once nothing runs them.
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
set B=%ROOT%\state\cxx\cb
set OUT=%1
if "%OUT%"=="" set OUT=%ROOT%\state\cxx\codebreaker\build
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
if not defined VCINSTALLDIR set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\native" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl || exit /b 1
cmake --build "%B%" --target codebreaker -j %JOBS% || exit /b 1
if not exist "%OUT%" mkdir "%OUT%"
del /q "%OUT%\codebreaker.old-*.exe" >nul 2>&1
if exist "%OUT%\codebreaker.exe" (
  2>nul (>>"%OUT%\codebreaker.exe" call ) || move /y "%OUT%\codebreaker.exe" "%OUT%\codebreaker.old-%RANDOM%%RANDOM%.exe" >nul
)
copy /y "%B%\bin\codebreaker.exe" "%OUT%\codebreaker.exe" >nul || exit /b 1
for %%D in (unicorn capstone libz3) do copy /y "%B%\bin\%%D.dll" "%OUT%\" >nul
rem the decompiler engine's programs, which the program starts from beside itself
for %%P in (decompiler pcodecheck) do copy /y "%B%\bin\%%P.exe" "%OUT%\" >nul
echo installed %OUT%\codebreaker.exe
exit /b 0
