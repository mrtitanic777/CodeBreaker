@echo off
rem build.cmd <worktree root> [force]  -- builds test.exe in this folder.
rem Everything inspect links besides its own port (the native readers, SQLite, the typed formats / corpus ports it imports) is built
rem once into ..\_p5_common\wt\<worktree folder>\cycle.lib by that folder's buildobj.cmd; the LE native reader and the typed
rem le / resources ports (not in that library) are compiled here.
set W=%~1
for %%a in ("%W%") do set CK=%%~nxa
call "%~dp0..\_p5_common\buildobj.cmd" "%W%" %2 || exit /b 1
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "%~dp0"
if not exist obj mkdir obj
del /q obj\*.obj 2>nul
set OPTS=/nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /I "%W%\native\engine" /I "%~dp0..\_p5_common"
set T=%W%\native\typed\cpp\codebreaker
cl %OPTS% /c "%W%\native\formats\le\le.cc" /Fo:obj\cbf_le.obj || exit /b 1
cl %OPTS% /c test.cpp "%T%\formats\inspect.cpp" "%T%\formats\le.cpp" "%T%\formats\resources.cpp" /Fo:obj\ || exit /b 1
link /nologo /OUT:test.exe obj\*.obj "%~dp0..\_p5_common\wt\%CK%\cycle.lib" bcrypt.lib advapi32.lib user32.lib ole32.lib shell32.lib || exit /b 1
exit /b 0
