@echo off
rem build.cmd <worktree root> [force]  -- builds test.exe in this folder.
rem Everything versions links besides its own port (the native readers, SQLite, the typed formats / corpus / store ports) is built once
rem into ..\_p5_common\wt\<worktree folder>\cycle.lib by that folder's buildobj.cmd.  Compiled here: the LE native reader, the typed ports
rem versions reads functions with (analyze.dos_functions, formats.pe_functions, jobs.pool) and formats.inspect with its le / resources;
rem capstone's headers come from Python's package.
rem stubs.cpp is TEST ONLY: verify::reconstruct::identity and decompile::describe::functions (the 16-bit starts) from knowledge.txt --
rem decompile.describe's own object pulls in the emulator and z3 through analyze.image (see the file).
set W=%~1
for %%a in ("%W%") do set CK=%%~nxa
call "%~dp0..\_p5_common\buildobj.cmd" "%W%" %2 || exit /b 1
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "%~dp0"
if not exist obj mkdir obj
del /q obj\*.obj 2>nul
for /f "delims=" %%i in ('python -c "import capstone,os;print(os.path.join(os.path.dirname(capstone.__file__),'include'))"') do set CSINC=%%i
for /f "delims=" %%i in ('python -c "import unicorn,os;print(os.path.join(os.path.dirname(unicorn.__file__),'include'))"') do set UCINC=%%i
set OPTS=/nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%CSINC%" /I "%UCINC%" /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /I "%W%\native\engine" /I "%W%\native\xex\src" /I "%~dp0..\_p5_common"
set T=%W%\native\typed\cpp\codebreaker
set N=%W%\native\formats
cl %OPTS% /c "%N%\le\le.cc" /Fo:obj\cbf_le.obj || exit /b 1
cl %OPTS% /c "%T%\build\data.cpp" "%T%\formats\nes.cpp" "%T%\decompile\m6502.cpp" "%T%\symbols\driver_services.cpp" "%T%\symbols\dos_interrupts.cpp" "%T%\analyze\dos_functions.cpp" /Fo:obj\ || exit /b 1
cl %OPTS% /c test.cpp stubs.cpp "%T%\compare\versions.cpp" "%T%\formats\pe_functions.cpp" "%T%\jobs\pool.cpp" "%T%\formats\inspect.cpp" "%T%\formats\le.cpp" "%T%\formats\resources.cpp" /Fo:obj\ || exit /b 1
link /nologo /OUT:test.exe obj\*.obj "%~dp0..\_p5_common\wt\%CK%\cycle.lib" ole32.lib advapi32.lib shell32.lib user32.lib bcrypt.lib || exit /b 1
exit /b 0
