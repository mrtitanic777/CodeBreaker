@echo off
rem clean_builds.cmd -- delete build output that can be rebuilt, and report the space freed. It never touches source,
rem git history, or the builds the running program uses (state\cxx\codebreaker, state\cxx\cb, native\<lib>\build of this
rem checkout -- the DLLs the Python loads live there).
rem
rem   - every worktree under .claude\worktrees: its state\cxx and native\<lib>\build (except native\cb\build, which is source)
rem   - state\cxx\cb-asan (the AddressSanitizer build) and old codebreaker.old-*.exe copies
rem   - compiler output (.obj .exe .pdb .lib .ilk .idb .pch .dll .exp) under state\work
rem   - state\test-tmp
setlocal
set ROOT=%~dp0..
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$root = (Resolve-Path '%ROOT%').Path; $before = ([IO.DriveInfo]::new($root.Substring(0,2))).AvailableFreeSpace;" ^
  "$dirs = @();" ^
  "$wt = Join-Path $root '.claude\worktrees'; if (Test-Path $wt) { foreach ($d in Get-ChildItem $wt -Directory) {" ^
  "  $c = Join-Path $d.FullName 'state\cxx'; if (Test-Path $c) { $dirs += $c };" ^
  "  $n = Join-Path $d.FullName 'native'; if (Test-Path $n) { foreach ($b in Get-ChildItem $n -Directory) { if ($b.Name -ne 'cb') { $bb = Join-Path $b.FullName 'build'; if (Test-Path $bb) { $dirs += $bb } } } } } };" ^
  "$asan = Join-Path $root 'state\cxx\cb-asan'; if (Test-Path $asan) { $dirs += $asan };" ^
  "$tt = Join-Path $root 'state\test-tmp'; if (Test-Path $tt) { $dirs += $tt };" ^
  "foreach ($p in $dirs) { $i = Get-Item $p -Force; if ($i.Attributes -band [IO.FileAttributes]::ReparsePoint) { Write-Host ('skipped link: ' + $p); continue }; cmd /c rmdir /s /q ('\"' + $p + '\"') 2>$null };" ^
  "Get-ChildItem (Join-Path $root 'state\cxx\codebreaker\build') -Filter 'codebreaker.old-*.exe' -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue;" ^
  "$work = Join-Path $root 'state\work'; if (Test-Path $work) { Get-ChildItem $work -Recurse -File -Include *.obj,*.exe,*.pdb,*.lib,*.ilk,*.idb,*.pch,*.dll,*.exp -ErrorAction SilentlyContinue | Where-Object { $_.FullName -notmatch '\\_tools\\' } | Remove-Item -Force -ErrorAction SilentlyContinue };" ^
  "$after = ([IO.DriveInfo]::new($root.Substring(0,2))).AvailableFreeSpace;" ^
  "'clean_builds: freed {0:N1} GB; {1:N1} GB free' -f (($after-$before)/1GB), ($after/1GB)"
