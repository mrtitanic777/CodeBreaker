param($folder, $devnames)
# mkdev.ps1 <harness folder> 'recon,reachability'  -- dev.cmd: relink with the objects of an earlier build, recompiling only test.cpp and the named sources
$t = Get-Content "$folder\build.cmd" -Raw
$dev = $devnames -split ','
$lines = $t -split "`r?`n"
$out = @()
foreach ($l in $lines) {
  if ($l -like 'cl %OPTS% test.cpp*') {
    $l = [regex]::Replace($l, '"%T%\\[^"]*\\(\w+)\.cpp"', { param($m) if (($dev -contains $m.Groups[1].Value) -or -not (Test-Path "$folder\obj\$($m.Groups[1].Value).obj")) { $m.Value } else { 'obj\' + $m.Groups[1].Value + '.obj' } })
    $l = [regex]::Replace($l, '"%W%\\native\\[a-z0-9\\]*\\(\w+)\.cc"', { param($m) if (Test-Path "$folder\obj\$($m.Groups[1].Value).obj") { 'obj\' + $m.Groups[1].Value + '.obj' } else { $m.Value } })
  }
  $out += $l
}
[IO.File]::WriteAllText("$folder\dev.cmd", ($out -join "`r`n"))
