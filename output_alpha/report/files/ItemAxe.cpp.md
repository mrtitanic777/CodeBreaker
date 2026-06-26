# ItemAxe.cpp

Risk score: **5**  ·  2 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L16` (rule, `ctor-stub`): constructor ItemAxe(int var1, int var2) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  ItemAxe(int var1, int var2) {...}
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L10` (marker, `PORT`): Block array init {Block::s_planks, Block::s_bookShelf, Block::s_wood, Block::s_crate} */;
  ```cpp
  Block** ItemAxe::s_blocksEffectiveAgainst = NULL /* PORT: Block array init {Block::s_planks, Block::s_bookShelf, Block::s_wood, Block::s_crate} */;
  ```
