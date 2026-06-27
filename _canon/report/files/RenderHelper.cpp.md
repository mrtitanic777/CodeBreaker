# RenderHelper.cpp

Risk score: **2**  ·  2 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `—` (emit, `static-init-ordered`): static field inits + static{} blocks deferred into initStatics(), run in dependency order by j2x_initAll() (kills the static-init-order crash class)

## PORTED — translated literally and cleanly

- **⚪ LOW** `—` (emit, `source`): generated source for RenderHelper
