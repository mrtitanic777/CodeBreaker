@echo off
rem gate.cmd [--no-build] [--exe PATH] [--no-unit] [--update] [--filter TEXT] -- the gate: what `codebreaker.exe tests\run_all.py --quick`
rem was, as native programs (docs/PYTHON_RETIREMENT.md 5.2).
rem
rem     tools\gate.cmd                 build native/cb (incremental: ninja's object cache is the compile cache), then in ONE parallel run
rem                                    (4 jobs, the project's cap): cb_tests as 8 shards + every command case of testdata\exe_gate
rem     tools\gate.cmd --no-build      skip the build (the objects are current)
rem     tools\gate.cmd --exe PATH      test this codebreaker.exe (default state\cxx\codebreaker\build\codebreaker.exe, else the main checkout's)
rem     tools\gate.cmd --no-unit       only the command cases (cb_exe_gate alone)
rem     tools\gate.cmd --filter lz32   only the cases whose name contains this (no unit tests)
rem     tools\gate.cmd --update ...    record goldens of the selected cases from the program's current output (review the diff before committing)
rem
rem Exit code 0 = pass. CB_BUILD_JOBS limits the build (default: half the cores); CB_GATE_JOBS the run (default 4).
setlocal
set HERE=%~dp0
set ROOT=%~dp0..
set B=%ROOT%\state\cxx\cb
set BUILD=1
set UNIT=1
set EXE=
set PASS=
:args
if "%1"=="" goto run
if "%1"=="--no-build" (set BUILD=0& shift& goto args)
if "%1"=="--no-unit" (set UNIT=0& shift& goto args)
if "%1"=="--exe" (set EXE=%2& shift& shift& goto args)
if "%1"=="--update" (set UNIT=0& set PASS=%PASS% --update& shift& goto args)
if "%1"=="--filter" (set UNIT=0& set PASS=%PASS% --filter %2& shift& shift& goto args)
set PASS=%PASS% %1
shift
goto args
:run
call "%HERE%disk_guard.cmd" || exit /b 1
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
set GJOBS=%CB_GATE_JOBS%
if "%GJOBS%"=="" set GJOBS=4
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\native" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1
set TARGETS=cb_exe_gate
if "%UNIT%"=="1" set TARGETS=cb_tests cb_exe_gate
if "%BUILD%"=="1" cmake --build "%B%" --target %TARGETS% -j %JOBS% || exit /b 1
set CMD="%B%\bin\cb_exe_gate.exe" --repo "%ROOT%" --jobs %GJOBS%
if not "%EXE%"=="" set CMD=%CMD% --exe "%EXE%"
if "%UNIT%"=="1" set CMD=%CMD% --unit "%B%\bin\cb_tests.exe"
%CMD% %PASS%
exit /b %ERRORLEVEL%
