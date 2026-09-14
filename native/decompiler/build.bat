@echo off
REM Build XEXMagic. Configures on first run (and whenever CMakeLists changes),
REM then builds. Pass a config to override Release, e.g. "build.bat Debug".

setlocal
set CFG=%1
if "%CFG%"=="" set CFG=Release

set CMAKE="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if not exist %CMAKE% set CMAKE=cmake

pushd "%~dp0"

if not exist build\XEXMagic.sln (
    %CMAKE% -S . -B build -G "Visual Studio 17 2022" -A x64 || goto :fail
)

%CMAKE% --build build --config %CFG% || goto :fail

echo.
echo Built: %~dp0build\bin\xexmagic.exe
popd
exit /b 0

:fail
echo.
echo BUILD FAILED
popd
exit /b 1
