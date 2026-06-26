# EffectRenderer.cpp

Risk score: **2**  ·  5 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L54` (marker, `PORT`): cap each FX layer to prevent runaway particle accumulation (TNT-stack OOM/fps).
  ```cpp
  // PORT: cap each FX layer to prevent runaway particle accumulation (TNT-stack OOM/fps).
  ```
- **⚪ LOW** `L141` (marker, `PORT`): guard unregistered block id (EntityDiggingFX derefs the Block)
  ```cpp
  if(var4 != 0 && Block::blocksList[var4] != NULL) {   // PORT: guard unregistered block id (EntityDiggingFX derefs the Block)
  ```
- **⚪ LOW** `L161` (marker, `PORT`): guard unregistered block id
  ```cpp
  if(var5 != 0 && Block::blocksList[var5] != NULL) {   // PORT: guard unregistered block id
  ```
- **⚪ LOW** `L196` (marker, `PORT`): original returned `"" + (count)` (Java string concat). Build the stats string
  ```cpp
  // PORT: original returned `"" + (count)` (Java string concat). Build the stats string
  ```
