$env:CB_WT='C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8'
cd 'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.ne_asm.sweep'
$sw=[Diagnostics.Stopwatch]::StartNew(); python expected.py 2>&1 | select -Last 12; 'py ' + $sw.Elapsed.TotalSeconds
$sw.Restart(); & .\run.cmd 2>&1 | select -Last 12; 'cpp ' + $sw.Elapsed.TotalSeconds
