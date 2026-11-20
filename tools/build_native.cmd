@echo off
rem build_native.cmd -- build what this checkout runs: native/cb's one build, and NASM until x86asm replaces it.
rem
rem     tools\build_native.cmd            configure (first time) and build both
rem     tools\build_native.cmd cb         only native/cb's build
rem     tools\build_native.cmd nasm       only NASM
rem
rem native/cb is one CMake build (state\cxx\cb): codebreaker.exe with every engine linked in as a library (the XEX
rem engine, the L2 core, the DOS executive, the Python parser ...), and the programs it starts from beside itself
rem (decompiler.exe, pcodecheck.exe) and cbtrace.dll, all in state\cxx\cb\bin. Every checkout and worktree has its own
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
if "%PARTS%"=="" set PARTS=cb nasm
set JOBS=%CB_BUILD_JOBS%
if "%JOBS%"=="" set /a JOBS=%NUMBER_OF_PROCESSORS% / 2
if %JOBS% LSS 1 set JOBS=1
set FAILED=
for %%L in (%PARTS%) do (
  if "%%L"=="cb" (
    echo == native/cb
    if not exist state\cxx\cb\build.ninja (
      cmake -S native -B state/cxx/cb -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl >nul || set FAILED=!FAILED! cb
    )
    if exist state\cxx\cb\build.ninja cmake --build state/cxx/cb --target codebreaker decompiler pcodecheck cbtrace -j !JOBS! || set FAILED=!FAILED! cb
  ) else if "%%L"=="nasm" (
    rem NASM is built with gcc (MinGW-w64): gcc on PATH, or MSYS2's at C:\msys64\mingw64\bin
    echo == native/nasm
    set MINGW=
    for %%G in (gcc.exe) do if not "%%~$PATH:G"=="" set MINGW=%%~dp$PATH:G
    if not defined MINGW if exist C:\msys64\mingw64\bin\gcc.exe set MINGW=C:\msys64\mingw64\bin\
    if not defined MINGW (
      echo    skipped: needs MinGW-w64 gcc ^(MSYS2: pacman -S mingw-w64-x86_64-gcc^)
    ) else (
      set "PATH=!MINGW!;!PATH!"
      if not exist native\nasm\build\CMakeCache.txt (
        cmake -S native/nasm -B native/nasm/build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc >nul || set FAILED=!FAILED! nasm
      )
      if exist native\nasm\build\CMakeCache.txt cmake --build native/nasm/build -j 2 || set FAILED=!FAILED! nasm
    )
  ) else (
    echo unknown part: %%L ^(cb or nasm^)
    set FAILED=!FAILED! %%L
  )
)
popd
if defined FAILED (echo FAILED:%FAILED% & exit /b 1)
echo all built
