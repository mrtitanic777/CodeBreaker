@echo off
rem build_native.cmd -- build every native library this checkout needs, in this checkout.
rem
rem     tools\build_native.cmd            configure (first time) and build each one
rem     tools\build_native.cmd l2 engine  only those
rem
rem Every checkout and every worktree has its own native/*/build (they are not in git), and the C back
rem end (cbl2.dll) and the engine and store (cbengine.dll) have no Python fallback any more, so a fresh
rem checkout runs this once before anything else. Release builds, Ninja, at most 2 parallel compiles.
setlocal enabledelayedexpansion
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~dp0..
pushd "%ROOT%" || exit /b 1
if not defined VCINSTALLDIR (
  set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
  call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || (echo vcvars64.bat failed & popd & exit /b 1)
)
set LIBS=%*
if "%LIBS%"=="" set LIBS=l2 engine cb formats dosexec symbols invert analyze xex nasm decompiler
set FAILED=
rem cb: native/cb's cbtrace.dll (the run recorder; sources in native/cb/emulate/tracer) into state\cxx\cb\bin, the
rem tracer_bin default. It is the tree tools\build_cb.cmd builds and tests; only that target is built here.
for %%L in (%LIBS%) do (
  if "%%L"=="cb" (
    echo == native/cb ^(cbtrace^)
    if not exist state\cxx\cb\build.ninja (
      cmake -S native -B state/cxx/cb -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl >nul || set FAILED=!FAILED! cb
    )
    if exist state\cxx\cb\build.ninja cmake --build state/cxx/cb --target cbtrace -j 2 || set FAILED=!FAILED! cb
  ) else if "%%L"=="nasm" (
    rem NASM is built with gcc (MinGW-w64): gcc on PATH, or MSYS2's at C:\msys64\mingw64\bin
    echo == native/nasm
    set MINGW=
    for %%G in (gcc.exe) do if not "%%~$PATH:G"=="" set MINGW=%%~dp$PATH:G
    if not defined MINGW if exist C:\msys64\mingw64\bin\gcc.exe set MINGW=C:\msys64\mingw64\bin\
    if not defined MINGW (
      echo    skipped: needs MinGW-w64 gcc ^(MSYS2: pacman -S mingw-w64-x86_64-gcc^); pe-source, ne-source and le-source use it
    ) else (
      set "PATH=!MINGW!;!PATH!"
      if not exist native\nasm\build\CMakeCache.txt (
        cmake -S native/nasm -B native/nasm/build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc >nul || set FAILED=!FAILED! nasm
      )
      if exist native\nasm\build\CMakeCache.txt cmake --build native/nasm/build -j 2 || set FAILED=!FAILED! nasm
    )
  ) else (
    echo == native/%%L
    rem (a tree configured before keeps its generator: --config Release covers Visual Studio's multi-config trees too)
    if not exist native\%%L\build\CMakeCache.txt (
      cmake -S native/%%L -B native/%%L/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl >nul || set FAILED=!FAILED! %%L
    )
    if exist native\%%L\build\CMakeCache.txt cmake --build native/%%L/build --config Release -j 2 || set FAILED=!FAILED! %%L
  )
)
popd
if defined FAILED (echo FAILED:%FAILED% & exit /b 1)
echo all built
