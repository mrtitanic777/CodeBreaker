# syntaxcheck.ps1 — compile every converted .cpp with the Xbox 360 XDK and
# aggregate compiler errors by category. Used to measure converter output quality.
#
# Usage:  pwsh -File syntaxcheck.ps1 [-Src <dir>]
#   -Src   directory of converted .cpp/.h (default: .\output)
#
# Emits:  _syntaxbuild\errors.log   (full per-file compiler output for failures)
#         _syntaxbuild\summary.txt  (error-code tally + per-file pass/fail)

param(
    [string]$Src = "$PSScriptRoot\output"
)

$ErrorActionPreference = 'Stop'
$XEDK = $env:XEDK
if (-not $XEDK) { $XEDK = "C:\Program Files (x86)\Microsoft Xbox 360 SDK" }
$env:INCLUDE = "$XEDK\include\xbox;$XEDK\include\xbox\sys;$XEDK\include\win32"
$env:LIB     = "$XEDK\lib\xbox;$XEDK\lib\win32"
$env:PATH    = "$XEDK\bin\win32;$env:PATH"

$bld = "$PSScriptRoot\_syntaxbuild"
New-Item -ItemType Directory -Force $bld | Out-Null
$errLog  = "$bld\errors.log"
$summary = "$bld\summary.txt"
Remove-Item $errLog,$summary -ErrorAction SilentlyContinue

$fi = "$XEDK\include\xbox\xbox_intellisense_platform.h"
$flags = @('/c','/nologo','/W3','/WX-','/Ox','/Os','/D','NDEBUG','/D','_XBOX',
           '/GF','/Gm-','/MT','/GS-','/Gy','/fp:fast','/fp:except-',
           '/Zc:wchar_t','/Zc:forScope','/GR-','/openmp-','/TP',"/FI$fi")

Push-Location $Src
try {
    Write-Host "Building PCH..."
    $pchOut = & cl.exe @flags /Yc"stdafx.h" /Fp"$bld\mc.pch" /Fo"$bld\\" stdafx.cpp 2>&1
    if ($LASTEXITCODE -ne 0) { $pchOut | Select-Object -Last 20; throw "PCH build failed" }

    $cpps = Get-ChildItem -Filter *.cpp | Where-Object { $_.Name -ne 'stdafx.cpp' } | Sort-Object Name
    Write-Host "Compiling $($cpps.Count) files..."

    $fail = 0; $pass = 0
    $codeTally = @{}
    $failFiles = New-Object System.Collections.Generic.List[string]

    foreach ($f in $cpps) {
        $o = & cl.exe @flags /Yu"stdafx.h" /Fp"$bld\mc.pch" /Fo"$bld\\" $f.Name 2>&1
        if ($LASTEXITCODE -ne 0) {
            $fail++
            $failFiles.Add($f.Name)
            Add-Content $errLog "===== $($f.Name) ====="
            $o | ForEach-Object { Add-Content $errLog $_ }
            # tally error codes (Cxxxx) only from lines marked 'error'
            $o | Select-String -Pattern 'error (C\d+)' -AllMatches | ForEach-Object {
                foreach ($m in $_.Matches) {
                    $code = $m.Groups[1].Value
                    if ($codeTally.ContainsKey($code)) { $codeTally[$code]++ } else { $codeTally[$code] = 1 }
                }
            }
        } else { $pass++ }
    }

    "Source dir: $Src"                                   | Tee-Object $summary
    "Compiled:   $($cpps.Count) files"                   | Tee-Object $summary -Append
    "PASS:       $pass"                                  | Tee-Object $summary -Append
    "FAIL:       $fail"                                  | Tee-Object $summary -Append
    ""                                                   | Tee-Object $summary -Append
    "=== Error code tally (total occurrences) ==="       | Tee-Object $summary -Append
    $codeTally.GetEnumerator() | Sort-Object Value -Descending | ForEach-Object {
        "{0,-8} {1}" -f $_.Key, $_.Value
    } | Tee-Object $summary -Append
    ""                                                   | Tee-Object $summary -Append
    "=== Failing files ==="                              | Tee-Object $summary -Append
    $failFiles | Tee-Object $summary -Append
}
finally { Pop-Location }
