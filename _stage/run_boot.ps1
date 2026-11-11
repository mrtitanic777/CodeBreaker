$env:CB_WT='C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8'; $env:CB_CYCLE_CK='agent-ab4ce2198a1860529'
Set-Location 'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.vxd.boot'
$sw=[Diagnostics.Stopwatch]::StartNew(); python expected.py 2>&1 | select -Last 8; 'boot py ' + $sw.Elapsed.TotalSeconds
$sw.Restart(); & .\run.cmd 2>&1 | select -Last 8; 'boot cpp ' + $sw.Elapsed.TotalSeconds
