# cycle.ps1 — fast CLEAN postfix iteration.
#
# Restores the pristine (pre-postfix) conversion, applies the current postfix passes,
# and (optionally) syntax-checks. Use this after editing a POSTFIX pass in
# java2xbox360.py — it avoids the ~27-min full reconversion by reusing the cached
# pristine, while staying clean (no in-place baked-in staleness).
#
#   pwsh -File cycle.ps1            # restore + postfix + syntaxcheck (full measure)
#   pwsh -File cycle.ps1 -NoCheck   # restore + postfix only (skip the compile measure)
#
# Prereq: output_pristine\ exists. Create/refresh it after a GENERATION change with:
#   python java2xbox360.py Java output_pristine --version indev --no-postfix
param([switch]$NoCheck)

$root = $PSScriptRoot
if (-not (Test-Path "$root\output_pristine\Block.cpp")) {
    Write-Host "ERROR: output_pristine missing. Build it with:`n  python java2xbox360.py Java output_pristine --version indev --no-postfix"
    exit 1
}

# Restore pristine -> output (conversion is skipped because files already exist)
Remove-Item "$root\output\*.cpp","$root\output\*.h" -ErrorAction SilentlyContinue
Copy-Item "$root\output_pristine\*.cpp","$root\output_pristine\*.h" "$root\output\" -Force

Push-Location $root
try {
    # Re-emits templates + runs postfix; Phase 2/3 conversion is skipped (files exist).
    python java2xbox360.py Java output --version indev --log output\build.log 2>&1 |
        Select-String "Postfix:|DONE" | Select-Object -Last 2
}
finally { Pop-Location }

if (-not $NoCheck) {
    pwsh -File "$root\syntaxcheck.ps1" 2>&1 | Select-String "PASS:|FAIL:"
}
