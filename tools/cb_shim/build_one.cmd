@echo off
rem build_one.cmd <worktree root> <out dir> <test.cpp> [extra sources ...]
rem Builds <out dir>\test.exe from an old harness's test.cpp against the compat shims (the headers under
rem native\typed\cpp that forward to native\cb) and the cb libraries (tools\build_cb.cmd builds them into
rem <worktree>\state\cxx\cb\lib). Scratch tooling for the exact.* refactors; nothing is written outside <out dir>.
rem With NATIVE set (sources), those are compiled first into obj\nat (names that collide with the extra sources).
set W=%~1
set O=%~2
set T=%~3
for /f "tokens=1,2,3,* delims= " %%a in ("%*") do set REST=%%d
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if not exist "%O%" mkdir "%O%"
if not exist "%O%\obj" mkdir "%O%\obj"
if not exist "%O%\obj\nat" mkdir "%O%\obj\nat"
set FLAGS=/nologo /std:c++20 /EHsc /utf-8 /permissive- /bigobj /O1 /MT /Zc:__cplusplus /Zc:char8_t- /DNOMINMAX /DWIN32_LEAN_AND_MEAN /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4267 /wd4996 /wd4018 /wd4146 /wd5051 /wd4101
set INCS=/I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /I "%W%" /I "%W%\native" /I "%W%\native\third_party" /I "%W%\native\third_party\include"
set NATOBJ=
if defined NATIVE (
  cl %FLAGS% %INCS% /c %NATIVE% /Fo:"%O%\obj\nat\\" || exit /b 1
  set NATOBJ="%O%\obj\nat\*.obj"
)
cl %FLAGS% %INCS% "%T%" %REST% %NATOBJ% /Fo:"%O%\obj\\" /Fe:"%O%\test.exe" /link /LIBPATH:"%W%\state\cxx\cb\lib" cb_exact.lib cb_core.lib bcrypt.lib
exit /b %errorlevel%
