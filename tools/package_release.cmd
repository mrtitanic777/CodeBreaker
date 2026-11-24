@echo off
rem package_release.cmd VERSION -- the release package, build\release\CodeBreaker-VERSION-win64.zip:
rem
rem     the committed source without its tests (git archive honours .gitattributes' export-ignore: tests\, every
rem     testdata\ folder and *_test.cpp, docs\history\), plus what runs it:
rem       bin\               codebreaker.exe, the DLLs it loads and the programs it starts (decompiler.exe, pcodecheck.exe)
rem       src\third_party\bin
rem
rem Build first (tools\build_codebreaker_exe.cmd) and commit: the package is made from HEAD and the built program.
setlocal
if "%~1"=="" (echo usage: package_release.cmd VERSION& exit /b 2)
set V=%~1
set HERE=%~dp0
for %%I in ("%HERE%..") do set ROOT=%%~fI
set NAME=CodeBreaker-%V%-win64
set REL=%ROOT%\build\release
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
for %%F in (codebreaker.exe decompiler.exe pcodecheck.exe unicorn.dll capstone.dll libz3.dll) do copy /y "%EXE%\%%F" "%OUT%\bin\" >nul || exit /b 1
mkdir "%OUT%\src\third_party\bin" 2>nul
copy /y "%ROOT%\src\third_party\bin\*" "%OUT%\src\third_party\bin\" >nul

rem the folders the archive left empty (testdata\ and tests\ whose files were all left out)
for /f "delims=" %%D in ('dir /ad /b /s "%OUT%" ^| sort /r') do rd "%%D" 2>nul
"%OUT%\bin\codebreaker.exe" help >nul 2>&1 || (echo the packaged program does not start& exit /b 1)
rem the start-up check made the program's work folder and its knowledge base; neither is part of the package
if exist "%OUT%\work" rmdir /s /q "%OUT%\work"
if exist "%OUT%\build" rmdir /s /q "%OUT%\build"
tar -a -cf "%REL%\%NAME%.zip" -C "%REL%" %NAME% || exit /b 1
echo packaged %REL%\%NAME%.zip
