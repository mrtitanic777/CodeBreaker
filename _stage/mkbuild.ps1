param($stage, $extra)
# mkbuild.ps1 <stage folder> '<extra typed sources, relative to %T%, comma separated>'
# the tool-lane build.cmd of the idiom_link harness with the module's own sources, and with its own native objects (obj_nat)
$W = 'C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8'
$src = Get-Content 'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.vxd.idiom_link\build.cmd' -Raw
$srcs = ($extra -split ',' | % { $e = $_.Trim(); if ($e.StartsWith('@')) { '"%W%\' + $e.Substring(1) + '"' } else { '"%T%\' + $e + '"' } }) -join ' '
$old = '"%T%\source\vxd\idiom.cpp" "%T%\source\vxd\link.cpp"'
if (-not $src.Contains($old)) { throw 'template changed' }
$src = $src.Replace($old, $srcs)
$src = $src.Replace('rem source\vxd\idiom.cpp + link.cpp (one unit)', 'rem the module sources + the tool lane (see source.vxd.idiom_link): source\vxd\idiom.cpp + link.cpp (one unit)')
$src = $src.Replace('for %%a in ("%W%") do set CK=%%~nxa', ('for %%a in ("%W%") do set CK=%%~nxa' + "`r`n" + 'if defined CB_CYCLE_CK set CK=%CB_CYCLE_CK%'))
# the native readers: here, in obj_nat (not shared with the other harnesses: their rebuild cannot empty it)
$src = $src.Replace('call "%~dp0..\_vxd_common\buildnat.cmd" "%W%" || exit /b 1' + "`r`n", '')
$nat = @(
'rem the native readers (native\formats, native\xex) are compiled here into obj_nat, as ..\_vxd_common\buildnat.cmd does but not shared',
'if exist obj_nat\le.obj goto natdone',
'mkdir obj_nat',
'for /f "delims=" %%i in (''python -c "import unicorn,os;print(os.path.join(os.path.dirname(unicorn.__file__),''include''))"'') do set UCINC2=%%i',
'cl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O2 /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /c "%W%\native\formats\le\le.cc" /Fo:obj_nat\ || exit /b 1',
'cl /nologo /std:c++17 /EHsc /utf-8 /permissive- /bigobj /O1 /I "%W%\native\ported\include" /I "%W%\native\typed\cpp" /I "%W%\native\l2\src" /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /I "%UCINC2%" /c "%W%\native\formats\dos\dos.cc" "%W%\native\formats\dos\unpack.cc" "%W%\native\formats\core\uc.cc" "%W%\native\formats\core\bytes.cc" "%W%\native\formats\core\text.cc" "%W%\native\formats\core\files.cc" "%W%\native\formats\core\codecs.cc" "%W%\native\formats\ne\ne.cc" "%W%\native\xex\src\xex_file.cpp" /Fo:obj_nat\ || exit /b 1',
':natdone') -join "`r`n"
$marker = 'if not exist obj_nd mkdir obj_nd'
if (-not $src.Contains($marker)) { throw 'marker missing' }
$src = $src.Replace($marker, $marker + "`r`n" + $nat)
$src = $src.Replace('set N=%~dp0..\_vxd_common\obj\nat', 'set N=%~dp0obj_nat')
[IO.File]::WriteAllText("$W\_stage\$stage\build.cmd", $src.Replace("`r`n", "`n").Replace("`n", "`r`n"))
