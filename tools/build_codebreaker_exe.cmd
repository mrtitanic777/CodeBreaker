@echo off
rem build_codebreaker_exe.cmd [fast] [OUT] -- CodeBreaker as one C++ program: every module of codebreaker/, the
rem tests and tools, the standard modules native/pyrt/stdlib.txt takes from CPython's Lib and the outside
rem packages native/pyrt/site.txt takes from site-packages, translated by pyengine --dynamic into OUT
rem (default state\cxx\codebreaker) and built with Ninja, 2 jobs, unity batches over a precompiled header.
rem
rem     tools\build_codebreaker_exe.cmd fast        /Od: OUT\build-fast\codebreaker.exe (~15 min cold)
rem     tools\build_codebreaker_exe.cmd             /O2: OUT\build\codebreaker.exe
rem
rem The program reads its command line as python does: codebreaker.exe <command> ... (python -m codebreaker),
rem codebreaker.exe -m pytest tests\java, codebreaker.exe tests\test_port.py, codebreaker.exe tests\run_all.py.
rem Needs tools\build_native.cmd pyengine pyrt first; CPython's Lib and site-packages are read at translation only.
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
set OUT=%2
if "%OUT%"=="" set OUT=%ROOT%\state\cxx\codebreaker
for /f "delims=" %%P in ('python -c "import sysconfig;print(sysconfig.get_path('stdlib'))"') do set PYLIB=%%P
for /f "delims=" %%P in ('python -c "import site;print(site.getusersitepackages())"') do set PYSITE=%%P
"%ROOT%\native\pyengine\build\bin\pyengine.exe" --dynamic --root "%ROOT%" --dyn-out "%OUT%" --dyn-main codebreaker --dyn-exe codebreaker ^
    --dyn-stdlib "%PYLIB%" --dyn-site "%PYSITE%" --dyn-tree codebreaker --dyn-files tests --dyn-files tools ^
    --dyn-files native\nasm\test\masm --dyn-native "%ROOT%\native\cbexe" --dyn-native-typed "%ROOT%\native\typed" 2>&1 | findstr /V /C:"refused: async"
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
set B=%OUT%\build
set EXTRA=
if "%1"=="fast" (
  set B=%OUT%\build-fast
  set EXTRA=-DCMAKE_CXX_FLAGS_RELEASE=/Od
)
if not exist "%B%\build.ninja" cmake -S "%OUT%" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release %EXTRA% >nul || exit /b 1
rem A codebreaker.exe that is running (the background service, an MCP server) cannot be overwritten, but it
rem can be renamed: moved aside, the link writes a new one and the running one goes on. Old ones are deleted
rem once nothing runs them.
del /q "%B%\codebreaker.old-*.exe" >nul 2>&1
if exist "%B%\codebreaker.exe" (
  2>nul (>>"%B%\codebreaker.exe" call ) || move /y "%B%\codebreaker.exe" "%B%\codebreaker.old-%RANDOM%%RANDOM%.exe" >nul
)
cmake --build "%B%" -j 2
exit /b %ERRORLEVEL%
