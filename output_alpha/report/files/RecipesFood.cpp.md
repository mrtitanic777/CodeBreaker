# RecipesFood.cpp

Risk score: **0**  ·  2 findings

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L21` (marker, `PORT`): array init {"Y", "X", "#", 'X', Block::s_mushroomBrown, 'Y', Block::s_mushroomRed, '#', Item::s_bowlEmpty} */);
  ```cpp
  var1->addRecipe(new ItemStack(Item::s_bowlSoup), NULL /* PORT: array init {"Y", "X", "#", 'X', Block::s_mushroomBrown, 'Y', Block::s_mushroomRed, '#', Item::s_b
  ```
- **⚪ LOW** `L22` (marker, `PORT`): array init {"Y", "X", "#", 'X', Block::s_mushroomRed, 'Y', Block::s_mushroomBrown, '#', Item::s_bowlEmpty} */);
  ```cpp
  var1->addRecipe(new ItemStack(Item::s_bowlSoup), NULL /* PORT: array init {"Y", "X", "#", 'X', Block::s_mushroomRed, 'Y', Block::s_mushroomBrown, '#', Item::s_b
  ```
