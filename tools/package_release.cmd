@echo off
rem package_release.cmd VERSION -- the release package, state\release\CodeBreaker-VERSION-win64.zip:
rem
rem     the committed source without its tests (git archive honours .gitattributes' export-ignore: tests\, every
rem     testdata\ folder and *_test.cpp, docs\history\), plus what runs it:
rem       bin\               codebreaker.exe and the DLLs it loads
rem       native\*\build\bin the engines it calls (decompiler, L2, DOS, XEX, symbols, invert, formats, engine, analyze, NASM)
rem       src\third_party\bin, src\java\retromcp\build\libs
rem
rem Build first (tools\build_codebreaker_exe.cmd) and commit: the package is made from HEAD and the built program.
setlocal
if "%~1"=="" (echo usage: package_release.cmd VERSION& exit /b 2)
set V=%~1
set HERE=%~dp0
for %%I in ("%HERE%..") do set ROOT=%%~fI
set NAME=CodeBreaker-%V%-win64
set REL=%ROOT%\state\release
set OUT=%REL%\%NAME%
set EXE=%ROOT%\build
if not exist "%EXE%\codebreaker.exe" (echo the program is not built: run tools\build_codebreaker_exe.cmd& exit /b 1)
if exist "%OUT%" rmdir /s /q "%OUT%"
if exist "%REL%\%NAME%.zip" del /q "%REL%\%NAME%.zip"
mkdir "%REL%" 2>nul

git -C "%ROOT%" archive --format=tar --prefix=%NAME%/ -o "%REL%\%NAME%.tar" HEAD || exit /b 1
tar -xf "%REL%\%NAME%.tar" -C "%REL%" || exit /b 1
del /q "%REL%\%NAME%.tar"

mkdir "%OUT%\bin"
for %%F in (codebreaker.exe unicorn.dll capstone.dll libz3.dll) do copy /y "%EXE%\%%F" "%OUT%\bin\" >nul || exit /b 1
for %%D in (decompiler l2 dosexec xex symbols invert formats engine analyze) do (
    if exist "%ROOT%\native\%%D\build\bin" (
        mkdir "%OUT%\native\%%D\build\bin" 2>nul
        for %%X in (exe dll) do if exist "%ROOT%\native\%%D\build\bin\*.%%X" copy /y "%ROOT%\native\%%D\build\bin\*.%%X" "%OUT%\native\%%D\build\bin\" >nul
    )
)
mkdir "%OUT%\src\third_party\bin" 2>nul
copy /y "%ROOT%\src\third_party\bin\*" "%OUT%\src\third_party\bin\" >nul
if exist "%ROOT%\src\java\retromcp\build\libs\retromcp.jar" (
    mkdir "%OUT%\src\java\retromcp\build\libs" 2>nul
    copy /y "%ROOT%\src\java\retromcp\build\libs\retromcp.jar" "%OUT%\src\java\retromcp\build\libs\" >nul
)

rem the folders the archive left empty (testdata\ and tests\ whose files were all left out)
for /f "delims=" %%D in ('dir /ad /b /s "%OUT%" ^| sort /r') do rd "%%D" 2>nul
"%OUT%\bin\codebreaker.exe" help >nul 2>&1 || (echo the packaged program does not start& exit /b 1)
if exist "%OUT%\state" rmdir /s /q "%OUT%\state"
tar -a -cf "%REL%\%NAME%.zip" -C "%REL%" %NAME% || exit /b 1
echo packaged %REL%\%NAME%.zip
