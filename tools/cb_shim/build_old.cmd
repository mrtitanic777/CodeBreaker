@echo off
rem build_old.cmd <worktree root> <out dir> <test.cpp> <old module .cpp> -- the OLD port (from the pre-refactor commit, in
rem state\work\cpp-refactor\_old) built into a harness exactly as the old build.cmd did (C++17, /O1), for timing.
set W=%~1
set O=%~2
set T=%~3
set M=%~4
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if not exist "%O%" mkdir "%O%"
if not exist "%O%\obj" mkdir "%O%\obj"
cl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%W%\state\work\cpp-refactor\_old" /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" "%T%" "%M%" "%W%\native\l2\src\pyre.cc" "%W%\native\l2\src\pystr.cc" "%W%\native\l2\src\value.cc" /Fo:"%O%\obj\\" /Fe:"%O%\test.exe"
exit /b %errorlevel%
