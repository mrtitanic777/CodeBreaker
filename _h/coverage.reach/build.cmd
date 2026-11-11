@echo off
rem build.cmd <worktree root> [force]  -- builds test.exe in this folder.
rem Everything reach links besides its own port (the native readers, SQLite, the typed formats / corpus / store ports) is built once
rem into ..\_p5_common\wt\<worktree folder>\cycle.lib by that folder's buildobj.cmd.  Compiled here: the LE and media / Xbox native readers
rem (native\formats: api, xbox\discs, xbox\assets, media\media, core\pyvalue, le; xex crypto\sha1) and the typed ports reach reads
rem containers with (formats.media, formats.xbox.volumes and the containers it opens), formats.inspect and its le / resources.
rem stubs.cpp is TEST ONLY (verify::reconstruct::identity from knowledge.txt: see the file).
set W=%~1
for %%a in ("%W%") do set CK=%%~nxa
call "%~dp0..\_p5_common\buildobj.cmd" "%W%" %2 || exit /b 1
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "%~dp0"
if not exist obj mkdir obj
del /q obj\*.obj 2>nul
for /f "delims=" %%i in ('python -c "import unicorn,os;print(os.path.join(os.path.dirname(unicorn.__file__),'include'))"') do set UCINC=%%i
set OPTS=/I "%UCINC%" /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /I "%W%\native\engine" /I "%W%\native\xex\src" /I "%~dp0..\_p5_common"
set T=%W%\native\typed\cpp\codebreaker
set N=%W%\native\formats
cl %OPTS% /c "%N%\le\le.cc" /Fo:obj\cbf_le.obj || exit /b 1
cl %OPTS% /c "%N%\api.cc" "%N%\xbox\discs.cc" "%N%\xbox\assets.cc" "%N%\media\media.cc" "%N%\core\pyvalue.cc" "%W%\native\xex\src\crypto\sha1.cpp" /Fo:obj\ || exit /b 1
rem (obj\media.obj is the native reader: the typed port's object is named apart)
cl %OPTS% /c "%T%\formats\media.cpp" /Fo:obj\typed_media.obj || exit /b 1
cl %OPTS% /c test.cpp stubs.cpp "%T%\coverage\reach.cpp" "%T%\formats\inspect.cpp" "%T%\formats\le.cpp" "%T%\formats\resources.cpp" /Fo:obj\ || exit /b 1
cl %OPTS% /c "%T%\formats\xbox\volumes.cpp" "%T%\formats\xbox\gdf.cpp" "%T%\formats\xbox\god.cpp" "%T%\formats\xbox\stfs.cpp" "%T%\formats\xbox\crypto\MemCrypto.cpp" "%T%\formats\xbox\fatx.cpp" "%T%\formats\xbox\nand.cpp" "%T%\formats\xbox\heavyiron.cpp" "%T%\formats\xbox\texture.cpp" "%T%\formats\xbox\pck.cpp" "%T%\formats\xbox\zar.cpp" /Fo:obj\ || exit /b 1
link /nologo /OUT:test.exe obj\*.obj "%~dp0..\_p5_common\wt\%CK%\cycle.lib" ole32.lib advapi32.lib shell32.lib user32.lib bcrypt.lib || exit /b 1
exit /b 0
