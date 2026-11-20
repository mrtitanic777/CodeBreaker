@echo off
rem build_native.cmd -- build what this checkout runs: src's one build.
rem
rem     tools\build_native.cmd            configure (first time) and build it
rem     tools\build_native.cmd cb         only src's build
rem
rem src is one CMake build (build): codebreaker.exe with every engine linked in as a library (the XEX
rem engine, the L2 core, the DOS executive, the Python parser ...), and the programs it starts from beside itself
rem (decompiler.exe, pcodecheck.exe) and cbtrace.dll, all in build. Every checkout and worktree has its own
rem (it is not in git), so a fresh checkout runs this once before anything else. Release builds, Ninja.
setlocal enabledelayedexpansion
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
pushd "%ROOT%" || exit /b 1
if not defined VCINSTALLDIR (
  set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
  call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || (echo vcvars64.bat failed & popd & exit /b 1)
)
set PARTS=%*
if "%PARTS%"=="" set PARTS=cb
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
set FAILED=
for %%L in (%PARTS%) do (
  if "%%L"=="cb" (
    echo == src
    if not exist build\build.ninja (
      cmake -S src -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl >nul || set FAILED=!FAILED! cb
    )
    if exist build\build.ninja cmake --build build --target codebreaker decompiler pcodecheck cbtrace -j !JOBS! || set FAILED=!FAILED! cb
  ) else (
    echo unknown part: %%L ^(cb^)
    set FAILED=!FAILED! %%L
  )
)
popd
if defined FAILED (echo FAILED:%FAILED% & exit /b 1)
echo all built
