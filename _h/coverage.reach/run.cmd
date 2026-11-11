@echo off
cd /d "%~dp0"
for %%i in ("%~dp0..\..\..\..") do set CB_CHECKOUT_ROOT=%%~fi
set COLUMNS=80
set PATH=%~dp0;%PATH%
if exist state rmdir /s /q state
robocopy seed state /E /NFL /NDL /NJH /NJS >nul
set CB_STATE=%~dp0state
set CB_FUNCTIONS_DB=%~dp0state\functions.sqlite
.\test.exe cases.txt > actual.txt
