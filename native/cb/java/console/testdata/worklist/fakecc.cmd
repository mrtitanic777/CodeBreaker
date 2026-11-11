@echo off
for %%a in (%*) do set LAST=%%~a
set LAST=%LAST:/=\%
type "%LAST%" 1>&2
