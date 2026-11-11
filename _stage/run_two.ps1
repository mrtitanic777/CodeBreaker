$env:CB_WT='C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8'; $env:CB_CYCLE_CK='agent-ab4ce2198a1860529'
foreach ($h in 'source.ne_asm.sweep','source.ne_asm.idiom') {
  cd 'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\'+$h
  $sw=[Diagnostics.Stopwatch]::StartNew(); python expected.py 2>&1 | select -Last 3; $h + ' py ' + $sw.Elapsed.TotalSeconds
  $sw.Restart(); & .\run.cmd 2>&1 | select -Last 3; $h + ' cpp ' + $sw.Elapsed.TotalSeconds
}
