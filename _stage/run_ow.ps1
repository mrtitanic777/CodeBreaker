$env:CB_WT='C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8'; $env:CB_CYCLE_CK='agent-ab4ce2198a1860529'
cd 'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.vxd.owbuild'
$sw=[Diagnostics.Stopwatch]::StartNew(); python expected.py 2>&1 | select -Last 3; 'py ' + $sw.Elapsed.TotalSeconds
$sw.Restart(); & .\run.cmd 2>&1 | select -Last 3; 'cpp ' + $sw.Elapsed.TotalSeconds
