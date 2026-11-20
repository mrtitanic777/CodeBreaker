@echo off
rem replace_running.cmd FILE -- before the build links FILE (codebreaker.exe, cb_tests.exe): the old one is deleted, or, when a
rem running process holds it (the background service, an MCP server, a test run), renamed aside to FILE.old-<n>.exe, which
rem Windows allows for a running program. The copies left aside are deleted the next time nothing runs them. Always succeeds.
setlocal
set F=%~1
for %%O in ("%~dpn1.old-*%~x1") do del /q "%%~fO" >nul 2>&1
if exist "%F%" del /q "%F%" >nul 2>&1
if exist "%F%" move /y "%F%" "%~dpn1.old-%RANDOM%%RANDOM%%~x1" >nul 2>&1
exit /b 0
