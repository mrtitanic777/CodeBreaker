@echo off
rem build.cmd <worktree root>  -- builds test.exe in this folder (the native readers: see ..\_vxd_common\buildnat.cmd).
set W=%~1
if "%W%"=="" (echo usage: build.cmd ^<worktree^> & exit /b 2)
call "%~dp0..\_vxd_common\buildnat.cmd" "%W%" || exit /b 1
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "%~dp0"
if not exist obj mkdir obj
set N=%~dp0..\_vxd_common\obj\nat
set OPTS=/nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src"
set T=%W%\native\typed\cpp\codebreaker
cl %OPTS% test.cpp "%T%\source\ne_asm\reachability.cpp" "%T%\source\ne_asm\datacode.cpp" "%T%\source\ne_asm\segment.cpp" "%T%\source\ne_asm\fixups.cpp" "%T%\formats\le.cpp" "%T%\formats\dos.cpp" "%T%\formats\detect.cpp" "%T%\base\corpus.cpp" "%T%\formats\ne.cpp" "%T%\formats\xex.cpp" "%T%\symbols\native.cpp" "%T%\build\exepack.cpp" "%W%\native\l2\src\pyre.cc" "%W%\native\l2\src\pystr.cc" "%W%\native\l2\src\value.cc" "%N%\le.obj" "%N%\dos.obj" "%N%\unpack.obj" "%N%\uc.obj" "%N%\bytes.obj" "%N%\text.obj" "%N%\files.obj" "%N%\codecs.obj" "%N%\ne.obj" "%N%\xex_file.obj" /Fo:obj\ /Fe:test.exe
exit /b %errorlevel%
