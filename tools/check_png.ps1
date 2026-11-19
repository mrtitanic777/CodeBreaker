# check_png.ps1 FILE WIDTH HEIGHT -- exit 0 when FILE is a PNG, not empty, of exactly WIDTH x HEIGHT pixels (tools\gate.cmd's screenshot step).
param([string]$File, [int]$Width, [int]$Height)
if (-not (Test-Path -LiteralPath $File)) { Write-Host "  $File : not written"; exit 1 }
$b = [System.IO.File]::ReadAllBytes($File)
if ($b.Length -lt 1000) { Write-Host "  $File : only $($b.Length) bytes"; exit 1 }
if (($b[0..7] -join ',') -ne '137,80,78,71,13,10,26,10') { Write-Host "  $File : not a PNG"; exit 1 }
$w = ([int]$b[16] -shl 24) -bor ([int]$b[17] -shl 16) -bor ([int]$b[18] -shl 8) -bor [int]$b[19]
$h = ([int]$b[20] -shl 24) -bor ([int]$b[21] -shl 16) -bor ([int]$b[22] -shl 8) -bor [int]$b[23]
if ($w -ne $Width -or $h -ne $Height) { Write-Host "  $File : ${w}x${h}, expected ${Width}x${Height}"; exit 1 }
exit 0
