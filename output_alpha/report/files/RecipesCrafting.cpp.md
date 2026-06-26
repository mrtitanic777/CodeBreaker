# RecipesCrafting.cpp

Risk score: **0**  ·  3 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L20` (marker, `PORT`): array init {"###", "# #", "###", '#', Block::s_planks} */);
  ```cpp
  var1->addRecipe(new ItemStack(Block::s_crate), NULL /* PORT: array init {"###", "# #", "###", '#', Block::s_planks} */);
  ```
- **⚪ LOW** `L21` (marker, `PORT`): array init {"###", "# #", "###", '#', Block::s_cobblestone} */);
  ```cpp
  var1->addRecipe(new ItemStack(Block::s_stoneOvenIdle), NULL /* PORT: array init {"###", "# #", "###", '#', Block::s_cobblestone} */);
  ```
- **⚪ LOW** `L22` (marker, `PORT`): array init {"##", "##", '#', Block::s_planks} */);
  ```cpp
  var1->addRecipe(new ItemStack(Block::s_workbench), NULL /* PORT: array init {"##", "##", '#', Block::s_planks} */);
  ```
