# EntityDiggingFX.cpp

Risk score: **2**  ·  2 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L33` (marker, `PORT`): s_grass is BlockGrass*; cast to Block* to compare
  ```cpp
  if(m_field_4082_a == (Block*)Block::s_grass) {   // PORT: s_grass is BlockGrass*; cast to Block* to compare
  ```
