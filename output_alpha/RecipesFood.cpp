#include "stdafx.h"
// Auto-generated from RecipesFood.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesFood.h"
#include "Common.h"
#include "CraftingManager.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"

RecipesFood::RecipesFood()
{
}

RecipesFood::~RecipesFood() {
	// No dynamic resources
}

void RecipesFood::func_976_a(CraftingManager* var1) {
		var1->addRecipe(new ItemStack(Item::s_bowlSoup), NULL /* PORT: array init {"Y", "X", "#", 'X', Block::s_mushroomBrown, 'Y', Block::s_mushroomRed, '#', Item::s_bowlEmpty} */);
		var1->addRecipe(new ItemStack(Item::s_bowlSoup), NULL /* PORT: array init {"Y", "X", "#", 'X', Block::s_mushroomRed, 'Y', Block::s_mushroomBrown, '#', Item::s_bowlEmpty} */);
}
