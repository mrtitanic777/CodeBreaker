@echo off
REM Rebuild LZ32.DLL from the recovered source with the from-source OpenWatcom v2
REM toolchain + our wthunk -- NO Microsoft tools. Proves the premise end-to-end on
REM an integral 32-bit flat-thunk component:
REM   lz32.c  -> wcc386        (32-bit veneer)
REM   lz32.rc -> wrc           (resources)
REM   lz.thk  -> wthunk + wasm (flat-thunk 32-bit half)
REM   *.obj/.res -> wlink      (PE DLL)
setlocal
set HERE=%~dp0
set SRC=C:\Users\mrtit\Documents\GitHub\Windows95\base\lz32
set BLD=%HERE%lz32-build
call "%HERE%owenv.bat"
if not exist "%BLD%" mkdir "%BLD%"
copy /y "%SRC%\lz32.c"   "%BLD%\" >nul
copy /y "%SRC%\lz32.rc"  "%BLD%\" >nul
copy /y "%SRC%\lz.thk"   "%BLD%\" >nul
copy /y "%HERE%lz32.lnk" "%BLD%\" >nul
cd /d "%BLD%"
echo [1/4] wcc386 lz32.c
wcc386 -bt=nt -q lz32.c            || goto err
echo [2/4] wrc lz32.rc
wrc -q -r -bt=nt lz32.rc           || goto err
echo [3/4] wthunk lz.thk + wasm (32-bit half)
python "%HERE%wthunk\wthunk.py" -o lz.asm lz.thk || goto err
wasm -zcm=masm -zzo -dIS_32=1 -fo=lz32t.obj lz.asm || goto err
echo [4/4] wlink -^> lz32.dll
wlink @lz32.lnk                    || goto err
echo.
echo BUILT: & dir lz32.dll | findstr lz32.dll
goto done
:err
echo BUILD FAILED (errorlevel %ERRORLEVEL%)
:done
endlocal
