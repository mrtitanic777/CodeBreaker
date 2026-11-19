@echo off
rem gate.cmd [--no-build] [--exe PATH] [--no-unit] [--no-app] [--update] [--filter TEXT] -- THE gate: cb_tests, every command case of cb_exe_gate against the real
rem codebreaker.exe, and the window's screenshot step (the window rendered in every state (including the open Language list and a translating run) and both themes, each a non-empty PNG of the size asked).
rem
rem     tools\gate.cmd                 build native/cb and codebreaker.exe (incremental: ninja's object cache is the compile cache), then in ONE parallel run
rem                                    (4 jobs, the project's cap): cb_tests as 8 shards + every command case of tests\exe_gate; then the screenshots
rem     tools\gate.cmd --no-build      skip the build (the objects are current)
rem     tools\gate.cmd --exe PATH      test this codebreaker.exe (default CB_EXE, else state\cxx\codebreaker\build\codebreaker.exe, which this script builds and installs)
rem     tools\gate.cmd --no-unit       only the command cases (cb_exe_gate alone) and the screenshots
rem     tools\gate.cmd --no-app        skip the screenshot step (--update, --filter likewise skip it)
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
set APP=1
set EXE=
set PASS=
set APP_RC=0
:args
if "%1"=="" goto run
if "%1"=="--no-build" (set BUILD=0& shift& goto args)
if "%1"=="--no-unit" (set UNIT=0& shift& goto args)
if "%1"=="--no-app" (set APP=0& shift& goto args)
if "%1"=="--exe" (set EXE=%2& shift& shift& goto args)
if "%1"=="--update" (set UNIT=0& set APP=0& set PASS=%PASS% --update& shift& goto args)
if "%1"=="--filter" (set UNIT=0& set APP=0& set PASS=%PASS% --filter %2& shift& shift& goto args)
set PASS=%PASS% %1
shift
goto args
:run
rem the program under test: --exe, else CB_EXE (the one setting every tool honors), else the one tools\build_codebreaker_exe.cmd installs
set OWN_EXE=0
if "%EXE%"=="" if defined CB_EXE set EXE=%CB_EXE%
if "%EXE%"=="" (set OWN_EXE=1& set EXE=%ROOT%\state\cxx\codebreaker\build\codebreaker.exe)
call "%HERE%disk_guard.cmd" || exit /b 1
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
set GJOBS=%CB_GATE_JOBS%
if "%GJOBS%"=="" set GJOBS=4
if not defined VCINSTALLDIR call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if not exist "%B%\build.ninja" cmake -S "%ROOT%\native" -B "%B%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl || exit /b 1
set TARGETS=cb_exe_gate
if "%UNIT%"=="1" set TARGETS=cb_tests cb_exe_gate
if "%BUILD%"=="1" cmake --build "%B%" --target %TARGETS% -j %JOBS% || exit /b 1
if "%BUILD%"=="1" if "%OWN_EXE%"=="1" call "%HERE%build_codebreaker_exe.cmd" || exit /b 1
set CMD="%B%\bin\cb_exe_gate.exe" --repo "%ROOT%" --jobs %GJOBS%
if not "%EXE%"=="" set CMD=%CMD% --exe "%EXE%"
if "%UNIT%"=="1" set CMD=%CMD% --unit "%B%\bin\cb_tests.exe"
%CMD% %PASS%
set RC=%ERRORLEVEL%
if "%APP%"=="1" call :app
if %APP_RC% NEQ 0 set RC=%APP_RC%
exit /b %RC%

rem The window (codebreaker.exe app) rendered to a PNG in each state and theme: the file must exist, be a PNG, not be empty, and be the size asked.
:app
set APP_RC=0
set SHOTS=%ROOT%\state\work\gate-shots
if exist "%SHOTS%" rmdir /s /q "%SHOTS%"
mkdir "%SHOTS%"
set T0=%TIME%
echo.
echo Window screenshots: codebreaker.exe app --screenshot, 8 states x 2 themes
for %%S in (idle working done unsupported language translating tools settings) do for %%T in (light dark) do call :shot %%S %%T
if %APP_RC% EQU 0 (echo Window screenshots: PASS ^(started %T0%, ended %TIME%^)) else echo Window screenshots: FAIL
if %APP_RC% EQU 0 rmdir /s /q "%SHOTS%"
exit /b %APP_RC%

:shot
"%EXE%" app --screenshot "%SHOTS%\%1-%2.png" --state %1 --theme %2 --size 1180x700 >nul || (echo   %1 %2: the program failed& set APP_RC=1& exit /b 1)
powershell -NoProfile -ExecutionPolicy Bypass -File "%HERE%check_png.ps1" "%SHOTS%\%1-%2.png" 1180 700 || set APP_RC=1
exit /b 0
