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
set ROOT=%~dp0..
pushd "%ROOT%" || exit /b 1
if not defined VCINSTALLDIR (
  call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || (echo vcvars64.bat failed & popd & exit /b 1)
)
set LIBS=%*
if "%LIBS%"=="" set LIBS=l2 engine tracer formats dosexec symbols invert analyze testkit
set FAILED=
for %%L in (%LIBS%) do (
  echo == native/%%L
  if not exist native\%%L\build\build.ninja (
    cmake -S native/%%L -B native/%%L/build -G Ninja -DCMAKE_BUILD_TYPE=Release >nul || set FAILED=!FAILED! %%L
  )
  if exist native\%%L\build\build.ninja cmake --build native/%%L/build -j 2 || set FAILED=!FAILED! %%L
)
if exist native\decompiler\build\bin\decompiler.exe (
  echo == native/decompiler: built
) else (
  echo == native/decompiler: not built here ^(Visual Studio generator; see docs/NATIVE.md^)
)
popd
if defined FAILED (echo FAILED:%FAILED% & exit /b 1)
echo all built
