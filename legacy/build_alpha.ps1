# Build the Alpha Xbox 360 XEX (Release_LTCG). Emits a "BUILD EXIT:" marker so callers
# can detect completion (the build uses /clp:NoSummary so there is no "Build succeeded" line).
param([string]$Log = "build_alpha.log")
$ErrorActionPreference = "Continue"
$msb  = "C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
$proj = "C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\MinecraftAlpha\MinecraftAlpha.vcxproj"
$env:XEDK = "C:\Program Files (x86)\Microsoft Xbox 360 SDK"
& $msb $proj /p:Configuration=Release_LTCG /p:Platform="Xbox 360" /m /nologo /clp:NoSummary `
    /flp:"logfile=$Log;verbosity=normal" 2>&1 | Out-Null
$code = $LASTEXITCODE
$errs = (Select-String -Path $Log -Pattern ": error " -SimpleMatch | Measure-Object).Count
Write-Output "BUILD EXIT: $code"
Write-Output "ERRORS: $errs"
