@echo off
rem disk_guard.cmd -- every build script calls this first: it refuses to start (exit 1) when the drive the checkout is on
rem has less than CB_MIN_FREE_GB free (default 30). Builds of the whole tree take gigabytes; on 2026-11-11 parallel
rem builds filled the drive from 150 GB to 5 GB. Set CB_MIN_FREE_GB=0 to turn the check off.
setlocal
set MIN=%CB_MIN_FREE_GB%
if "%MIN%"=="" set MIN=30
if "%MIN%"=="0" exit /b 0
set FREE=
for /f %%F in ('powershell -NoProfile -Command "[int][math]::Floor(([IO.DriveInfo]::new('%~d0')).AvailableFreeSpace/1GB)"') do set FREE=%%F
if "%FREE%"=="" exit /b 0
if %FREE% LSS %MIN% (
  echo disk_guard: only %FREE% GB free on %~d0 ^(need %MIN% GB^). Delete build output first: tools\clean_builds.cmd
  exit /b 1
)
exit /b 0
