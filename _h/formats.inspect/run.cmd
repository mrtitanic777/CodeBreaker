@echo off
cd /d "%~dp0"
for %%i in ("%~dp0..\..\..\..") do set CB_CHECKOUT_ROOT=%%~fi
.\test.exe cases.txt > actual.txt
