param($stage, $name)
$W='C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8'; $T='C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port'
New-Item -ItemType Directory -Force "$T\_ne_common","$T\$name" | Out-Null
Copy-Item "$W\_stage\_ne_common\*" "$T\_ne_common\" -Force
Copy-Item "$W\_stage\$stage\*" "$T\$name\" -Force
