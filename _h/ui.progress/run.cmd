@echo off
cd /d "%~dp0"
for %%i in ("%~dp0..\..\..\..") do set CB_CHECKOUT_ROOT=%%~fi
if exist work rmdir /s /q work
robocopy seed work /E /NFL /NDL /NJH /NJS >nul
.\test.exe cases.txt > actual.txt
