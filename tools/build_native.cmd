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
  call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || (echo vcvars64.bat failed & popd & exit /b 1)
)
set LIBS=%*
if "%LIBS%"=="" set LIBS=l2 engine cb formats dosexec symbols invert analyze testkit
set FAILED=
rem cb: native/cb's cbtrace.dll (the run recorder; sources in native/cb/emulate/tracer) into state\cxx\cb\bin, the
rem tracer_bin default. It is the tree tools\build_cb.cmd builds and tests; only that target is built here.
for %%L in (%LIBS%) do (
  if "%%L"=="cb" (
    echo == native/cb ^(cbtrace^)
    if not exist state\cxx\cb\build.ninja (
      cmake -S native -B state/cxx/cb -G Ninja -DCMAKE_BUILD_TYPE=Release >nul || set FAILED=!FAILED! cb
    )
    if exist state\cxx\cb\build.ninja cmake --build state/cxx/cb --target cbtrace -j 2 || set FAILED=!FAILED! cb
  ) else (
    echo == native/%%L
    if not exist native\%%L\build\build.ninja (
      cmake -S native/%%L -B native/%%L/build -G Ninja -DCMAKE_BUILD_TYPE=Release >nul || set FAILED=!FAILED! %%L
    )
    if exist native\%%L\build\build.ninja cmake --build native/%%L/build -j 2 || set FAILED=!FAILED! %%L
  )
)
if exist native\decompiler\build\bin\decompiler.exe (
  echo == native/decompiler: built
) else (
  echo == native/decompiler: not built here ^(Visual Studio generator; see docs/NATIVE.md^)
)
popd
if defined FAILED (echo FAILED:%FAILED% & exit /b 1)
echo all built
