# Build Minecraft360Converter.exe — a double-clickable, no-Python-needed desktop app.
# Run:  pwsh -File build_gui.ps1     (or:  python -m PyInstaller ... as below)
# Output: ConversionTool\dist\Minecraft360Converter.exe
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot

python -m PyInstaller --onefile --windowed --noconfirm `
  --name Minecraft360Converter `
  --add-data "handwritten;handwritten" `
  --hidden-import java2xbox360 `
  --hidden-import update_version `
  --hidden-import converter_lint `
  converter_gui.py

# Custom version profiles must be USER-EDITABLE, so they live NEXT TO the .exe (not baked into the
# one-file bundle, which unpacks read-only to a temp dir). Seed dist\profiles with the README + any
# profiles already authored, without clobbering ones the user has added there.
$srcProfiles = Join-Path $PSScriptRoot 'profiles'
$dstProfiles = Join-Path $PSScriptRoot 'dist\profiles'
if (Test-Path $srcProfiles) {
  New-Item -ItemType Directory -Force $dstProfiles | Out-Null
  Copy-Item (Join-Path $srcProfiles '*') $dstProfiles -Recurse -Force
}

Write-Host ""
Write-Host "Built: $PSScriptRoot\dist\Minecraft360Converter.exe"
Write-Host "Profiles folder (add custom versions here): $dstProfiles"
Write-Host "Double-click the .exe, or pin it to the taskbar."
