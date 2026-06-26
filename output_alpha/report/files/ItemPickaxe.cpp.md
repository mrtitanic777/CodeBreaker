# ItemPickaxe.cpp

Risk score: **0**  ·  1 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L11` (marker, `PORT`): Block array init {Block::s_cobblestone, Block::s_stairDouble, Block::s_stairSingle, Block::s_stone, Block::s_cobblestoneMossy, Block::s_oreIron, Block::s_blockSteel, Block::s_oreCoal, Block::s_blockGold, Block::s_oreGold, Block::s_oreDiamond, Block::s_blockDiamond, Block::s_blockIce, Block::s_bloodStone} */;
  ```cpp
  Block** ItemPickaxe::s_blocksEffectiveAgainst = NULL /* PORT: Block array init {Block::s_cobblestone, Block::s_stairDouble, Block::s_stairSingle, Block::s_stone
  ```
