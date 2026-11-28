@echo off
rem tools\sdk\samples.cmd [--update] -- the SDK's step of the gate (tools\gate.cmd): builds cbsdk.dll, installs the SDK (build\sdk: bin,
rem include\cb, lib, lib\cmake\CodeBreaker, share\codebreaker), builds samples\ against that installed SDK, never the source tree
rem (build\samples), runs every case below and compares what it prints with tests\sdk\golden\<case>.txt.
rem
rem     tools\sdk\samples.cmd             a case passes when it exits 0 and prints its golden
rem     tools\sdk\samples.cmd --update    records the goldens from what the samples print now (review the diff before committing)
rem
rem The samples run as a program of someone else's would: the SDK's bin on PATH, none of CodeBreaker's settings in the environment, a state
rem folder of their own (build\sdk-run\state, kept between runs: the decompiler's results are cached there), TEMP under build\sdk-run.
setlocal
set ROOT=%~dp0..\..
for %%I in ("%ROOT%") do set ROOT=%%~fI
set B=%ROOT%\build
set PREFIX=%B%\sdk
set SB=%B%\samples
set RUN=%B%\sdk-run
set GOLD=%ROOT%\tests\sdk\golden
set FMT=%ROOT%\src\formats\testdata
set UPDATE=0
if "%1"=="--update" set UPDATE=1
set FAILED=0
set T0=%TIME%
if not defined VCINSTALLDIR set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\src" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl || exit /b 1
cmake --build "%B%" --target cbsdk || exit /b 1
cmake --install "%B%" --component sdk --prefix "%PREFIX%" >nul || exit /b 1
if not exist "%SB%\build.ninja" cmake -S "%ROOT%\samples" -B "%SB%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="%PREFIX%" -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl >nul || exit /b 1
cmake --build "%SB%" || exit /b 1

rem the samples' surroundings: the installed SDK alone
set "PATH=%PREFIX%\bin;%PATH%"
for %%V in (CB_CHECKOUT_ROOT CB_STATE CB_KNOWLEDGE_DB CB_LANGUAGES_DIR CB_DECOMPILER_BIN CB_THIRD_PARTY_BIN CB_CONVERTED) do set %%V=
if not exist "%RUN%\state" mkdir "%RUN%\state"
if exist "%RUN%\out" rmdir /s /q "%RUN%\out"
mkdir "%RUN%\out"
if not exist "%RUN%\tmp" mkdir "%RUN%\tmp"
set TEMP=%RUN%\tmp
set TMP=%RUN%\tmp
if "%UPDATE%"=="1" if not exist "%GOLD%" mkdir "%GOLD%"

echo.
echo SDK samples: built against %PREFIX%
call :case HelloBinary-pe HelloBinary\HelloBinary.exe "%FMT%\lz32.dll"
call :case HelloBinary-ne HelloBinary\HelloBinary.exe "%FMT%\lzexpand.dll"
call :case HelloDecompile-pe HelloDecompile\HelloDecompile.exe "%FMT%\lz32.dll" LZCopy
call :case HelloDecompile-ne HelloDecompile\HelloDecompile.exe "%FMT%\lzexpand.dll" LZCOPY
call :case HelloDecompile-address HelloDecompile\HelloDecompile.exe "%FMT%\lzexpand.dll" 10:0124
call :case HelloDecompile-module HelloDecompile\HelloDecompile.exe "%FMT%\lz32.dll" --module
call :case DisasmAnyCpu-tour DisasmAnyCpu\DisasmAnyCpu.exe
call :case DisasmAnyCpu-bytes DisasmAnyCpu\DisasmAnyCpu.exe z80 3E12CD3412C9 0x100
call :case DisasmAnyCpu-binary DisasmAnyCpu\DisasmAnyCpu.exe --binary "%FMT%\lz32.dll" 12
if %FAILED% EQU 0 (echo SDK samples: PASS ^(started %T0%, ended %TIME%^)) else echo SDK samples: FAIL
if %FAILED% EQU 0 rmdir /s /q "%RUN%\out"
exit /b %FAILED%

rem :case NAME EXE ARGS... -- runs build\samples\EXE with ARGS (state in build\sdk-run\state) and compares its stdout with the golden
:case
set NAME=%1
set EXE=%2
shift
shift
set ARGS=
:case_args
if "%~1"=="" goto case_run
set ARGS=%ARGS% %1
shift
goto case_args
:case_run
"%SB%\%EXE%" --state "%RUN%\state"%ARGS% > "%RUN%\out\%NAME%.txt" 2> "%RUN%\out\%NAME%.err"
if errorlevel 1 (
  echo   %NAME%: FAIL, exit code %ERRORLEVEL%
  type "%RUN%\out\%NAME%.err"
  set FAILED=1
  exit /b 0
)
if "%UPDATE%"=="1" (
  copy /y "%RUN%\out\%NAME%.txt" "%GOLD%\%NAME%.txt" >nul
  echo   %NAME%: recorded
  exit /b 0
)
fc /L "%RUN%\out\%NAME%.txt" "%GOLD%\%NAME%.txt" >nul 2>&1
if errorlevel 1 (
  echo   %NAME%: FAIL, its output is not tests\sdk\golden\%NAME%.txt:
  fc /L /N "%RUN%\out\%NAME%.txt" "%GOLD%\%NAME%.txt"
  set FAILED=1
  exit /b 0
)
echo   %NAME%: ok
exit /b 0
