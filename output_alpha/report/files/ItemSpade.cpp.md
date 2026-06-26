# ItemSpade.cpp

Risk score: **5**  ·  2 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L16` (rule, `ctor-stub`): constructor ItemSpade(int var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemSpade(int var1, int var2) {...}
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L10` (marker, `PORT`): Block array init {Block::s_grass, Block::s_dirt, Block::s_sand, Block::s_gravel, Block::s_snow, Block::s_blockSnow, Block::s_blockClay} */;
  ```cpp
  Block** ItemSpade::s_blocksEffectiveAgainst = NULL /* PORT: Block array init {Block::s_grass, Block::s_dirt, Block::s_sand, Block::s_gravel, Block::s_snow, Bloc
  ```
