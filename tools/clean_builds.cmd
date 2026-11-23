@echo off
rem clean_builds.cmd -- delete build output that can be rebuilt, and report the space freed. It never touches source,
rem git history, or the builds the running program uses (the build folder of this checkout).
rem
rem   - every worktree under .claude\worktrees: its build folder
rem   - build-asan (the AddressSanitizer build) and old codebreaker.old-*.exe copies
rem   - compiler output (.obj .exe .pdb .lib .ilk .idb .pch .dll .exp) under work
setlocal
set ROOT=%~dp0..
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$root = (Resolve-Path '%ROOT%').Path; $before = ([IO.DriveInfo]::new($root.Substring(0,2))).AvailableFreeSpace;" ^
  "$dirs = @();" ^
  "$wt = Join-Path $root '.claude\worktrees'; if (Test-Path $wt) { foreach ($d in Get-ChildItem $wt -Directory) {" ^
  "  $c = Join-Path $d.FullName 'build'; if (Test-Path $c) { $dirs += $c } } };" ^
  "$asan = Join-Path $root 'build-asan'; if (Test-Path $asan) { $dirs += $asan };" ^
  "foreach ($p in $dirs) { $i = Get-Item $p -Force; if ($i.Attributes -band [IO.FileAttributes]::ReparsePoint) { Write-Host ('skipped link: ' + $p); continue }; cmd /c rmdir /s /q ('\"' + $p + '\"') 2>$null };" ^
  "Get-ChildItem (Join-Path $root 'build') -Filter 'codebreaker.old-*.exe' -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue;" ^
  "$work = Join-Path $root 'work'; if (Test-Path $work) { Get-ChildItem $work -Recurse -File -Include *.obj,*.exe,*.pdb,*.lib,*.ilk,*.idb,*.pch,*.dll,*.exp -ErrorAction SilentlyContinue | Where-Object { $_.FullName -notmatch '\\_tools\\' } | Remove-Item -Force -ErrorAction SilentlyContinue };" ^
  "$after = ([IO.DriveInfo]::new($root.Substring(0,2))).AvailableFreeSpace;" ^
  "'clean_builds: freed {0:N1} GB; {1:N1} GB free' -f (($after-$before)/1GB), ($after/1GB)"
