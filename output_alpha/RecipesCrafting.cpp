#include "stdafx.h"
// Auto-generated from RecipesCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesCrafting.h"
#include "Common.h"
#include "CraftingManager.h"
#include "Block.h"
#include "ItemStack.h"

RecipesCrafting::RecipesCrafting()
{
}

RecipesCrafting::~RecipesCrafting() {
	// No dynamic resources
}

void RecipesCrafting::func_1051_a(CraftingManager* var1) {
		var1->addRecipe(new ItemStack(Block::s_crate), NULL /* PORT: array init {"###", "# #", "###", '#', Block::s_planks} */);
		var1->addRecipe(new ItemStack(Block::s_stoneOvenIdle), NULL /* PORT: array init {"###", "# #", "###", '#', Block::s_cobblestone} */);
		var1->addRecipe(new ItemStack(Block::s_workbench), NULL /* PORT: array init {"##", "##", '#', Block::s_planks} */);
}
