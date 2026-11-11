@echo off
rem build_rt_exe.cmd [checkout root] -- rebuilds the Python-compatible runtime (native\pyrt) and codebreaker.exe,
rem the two steps before the exe gate (codebreaker.exe tests\run_all.py --quick).  Default root: this script's checkout.
setlocal
call "%~dp0disk_guard.cmd" || exit /b 1
set ROOT=%~1
if "%ROOT%"=="" set ROOT=%~dp0..
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
cd /d "%ROOT%" || exit /b 1
echo [%time%] pyrt
cmake --build native\pyrt\build -j 2 || exit /b 2
echo [%time%] exe
call tools\build_codebreaker_exe.cmd || exit /b 3
echo [%time%] done
