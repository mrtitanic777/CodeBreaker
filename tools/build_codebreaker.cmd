@echo off
rem build_codebreaker.cmd [OUT] -- the plain native program: every command of codebreaker/__main__.py on native/cb, no Python behind it
rem (docs/PYTHON_RETIREMENT.md; the terminal interface stays Python and is started by the program under real python).
rem
rem     tools\build_codebreaker.cmd          builds target cb_codebreaker in state\cxx\cb (the same build tree as tools\build_cb.cmd, so the objects
rem                                          are shared) and installs it with the DLLs it loads into state\cxx\codebreaker\native\
rem     tools\build_codebreaker.cmd OUT      installs into OUT instead
rem
rem The result, state\cxx\codebreaker\native\codebreaker.exe, sits beside the translated-Python program (state\cxx\codebreaker\build\codebreaker.exe,
rem tools\build_codebreaker_exe.cmd, untouched) until the plain one is proven; pointing a tool at it is one setting: CB_EXE=<that path>.
rem Needs MSVC, CMake, Ninja. CB_BUILD_JOBS limits the build (default half the cores). A running codebreaker.exe is moved aside, not overwritten.
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
set B=%ROOT%\state\cxx\cb
set OUT=%1
if "%OUT%"=="" set OUT=%ROOT%\state\cxx\codebreaker\native
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\native" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1
cmake --build "%B%" --target cb_codebreaker -j %JOBS% || exit /b 1
if not exist "%OUT%" mkdir "%OUT%"
rem the build names the program codebreaker_cb.exe; it is installed as codebreaker.exe. A running one cannot be overwritten but can be renamed.
del /q "%OUT%\codebreaker.old-*.exe" >nul 2>&1
if exist "%OUT%\codebreaker.exe" (
  2>nul (>>"%OUT%\codebreaker.exe" call ) || move /y "%OUT%\codebreaker.exe" "%OUT%\codebreaker.old-%RANDOM%%RANDOM%.exe" >nul
)
copy /y "%B%\bin\codebreaker_cb.exe" "%OUT%\codebreaker.exe" >nul || exit /b 1
for %%D in (unicorn capstone libz3) do copy /y "%B%\bin\%%D.dll" "%OUT%\" >nul
echo installed %OUT%\codebreaker.exe
exit /b 0
