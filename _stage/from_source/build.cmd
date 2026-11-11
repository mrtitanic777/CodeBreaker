@echo off
rem build.cmd <worktree root>  -- builds test.exe in this folder.
rem the module sources + the tool lane (see source.vxd.idiom_link): source\vxd\idiom.cpp + link.cpp (one unit) with masm.cpp %W%\native\ported\src\py_zlib.cc, le_writer.cpp, the disassembly and the readers (see ..\_vxd_common\buildnat.cmd),
rem and for the tool lane build\dos_tools, build\native_dos, build\dos_sessions, build\masm, the typed DOS executive over native\dosexec
rem (compiled into obj_nd) and the cycle's library (..\_cycle_common\wt\%CK%\cycle.lib; built by ..\_cycle_common\buildobj.cmd).
rem Object names that two ports share (masm.cpp of build\ and of source\vxd\, dos.cpp of formats\ and of exact\emu\) are compiled apart.
set W=%~1
if "%W%"=="" (echo usage: build.cmd ^<worktree^> & exit /b 2)
set CC=C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_cycle_common
for %%a in ("%W%") do set CK=%%~nxa
if defined CB_CYCLE_CK set CK=%CB_CYCLE_CK%
if not exist "%CC%\wt\%CK%\cycle.lib" call "%CC%\buildobj.cmd" "%W%" || exit /b 1
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "%~dp0"
if not exist obj mkdir obj
if not exist obj_vxd mkdir obj_vxd
if not exist obj_emu mkdir obj_emu
if not exist obj_nd mkdir obj_nd
rem the native readers (native\formats, native\xex) are compiled here into obj_nat, as ..\_vxd_common\buildnat.cmd does but not shared
if exist obj_nat\le.obj goto natdone
mkdir obj_nat
for /f "delims=" %%i in ('python -c "import unicorn,os;print(os.path.join(os.path.dirname(unicorn.__file__),'include'))"') do set UCINC2=%%i
cl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O2 /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /c "%W%\native\formats\le\le.cc" /Fo:obj_nat\ || exit /b 1
cl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%UCINC2%" /c "%W%\native\formats\dos\dos.cc" "%W%\native\formats\dos\unpack.cc" "%W%\native\formats\core\uc.cc" "%W%\native\formats\core\bytes.cc" "%W%\native\formats\core\text.cc" "%W%\native\formats\core\files.cc" "%W%\native\formats\core\codecs.cc" "%W%\native\formats\ne\ne.cc" "%W%\native\xex\src\xex_file.cpp" /Fo:obj_nat\ || exit /b 1
:natdone
for /f "delims=" %%i in ('python -c "import capstone,os;print(os.path.join(os.path.dirname(capstone.__file__),'include'))"') do set CSINC=%%i
for /f "delims=" %%i in ('python -c "import unicorn,os;print(os.path.join(os.path.dirname(unicorn.__file__),'include'))"') do set UCI=%%i
set ND=%W%\native\dosexec
if not exist obj_nd\exec.obj cl /nologo /c /std:c++17 /EHsc /utf-8 /O2 /W3 /DNOMINMAX /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /I "%ND%" /I "%UCI%" "%ND%\exec.cc" "%ND%\dpmi.cc" "%ND%\fs.cc" "%ND%\mcb.cc" "%ND%\pyfmt.cc" "%ND%\uc.cc" /Fo:obj_nd\ || exit /b 1
set N=%~dp0obj_nat
set T=%W%\native\typed\cpp\codebreaker
set OPTS=/nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /I "%W%\native\engine" /I "%CC%" /I "%W%\native\engine\third_party\sqlite" /I "%ND%" /I "%UCI%" /I "%CSINC%"
cl %OPTS% /c "%T%\source\vxd\masm.cpp" /Fo:obj_vxd\ || exit /b 1
cl %OPTS% /c "%T%\exact\emu\dos.cpp" /Fo:obj_emu\ || exit /b 1
cl %OPTS% test.cpp obj_vxd\masm.obj obj_emu\dos.obj "%T%\source\vxd\from_source.cpp" "%T%\source\vxd\idiom.cpp" "%T%\source\vxd\link.cpp" "%W%\native\ported\src\py_zlib.cc" "%T%\source\vxd\le_writer.cpp" "%T%\source\vxd\disassembly.cpp" "%T%\source\vxd\services.cpp" "%T%\source\vxd\symbols.cpp" "%T%\build\dos_tools.cpp" "%T%\build\masm.cpp" "%T%\build\native_dos.cpp" "%T%\build\dos_sessions.cpp" "%T%\jobs\pool.cpp" "%T%\base\cache.cpp" "%T%\formats\le.cpp" "%T%\formats\dos.cpp" "%T%\formats\detect.cpp" "%T%\base\corpus.cpp" "%T%\formats\ne.cpp" "%T%\formats\xex.cpp" "%T%\symbols\native.cpp" "%T%\build\exepack.cpp" "%T%\exact\emu\native_dosexec.cpp" "%T%\exact\emu\dpmi.cpp" "%T%\exact\emu\xenix.cpp" "%W%\native\l2\src\pyre.cc" "%W%\native\l2\src\pystr.cc" "%W%\native\l2\src\value.cc" "%N%\le.obj" "%N%\dos.obj" "%N%\unpack.obj" "%N%\uc.obj" "%N%\bytes.obj" "%N%\text.obj" "%N%\files.obj" "%N%\codecs.obj" "%N%\ne.obj" "%N%\xex_file.obj" obj_nd\exec.obj obj_nd\dpmi.obj obj_nd\fs.obj obj_nd\mcb.obj obj_nd\pyfmt.obj obj_nd\uc.obj "%CC%\wt\%CK%\cycle.lib" bcrypt.lib advapi32.lib user32.lib /Fo:obj\ /Fe:test.exe
exit /b %errorlevel%
