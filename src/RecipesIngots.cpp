#include "stdafx.h"
// Auto-generated from RecipesIngots.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesIngots.h"
#include "Common.h"
#include "CraftingManager.h"
#include "Block.h"
#include "Item.h"
#include "ItemStack.h"

RecipesIngots::RecipesIngots()
	: m_recipeItems(new void* /* PORT: was Object */[][])
{
}

RecipesIngots::~RecipesIngots() {
	delete[] m_recipeItems;
	m_recipeItems = NULL;
}

void RecipesIngots::addRecipes(CraftingManager* var1) {
		for(int var2 = 0; var2 < m_recipeItems_count /* PORT: set this from allocation */; ++var2) {
			Block var3 = (Block*)m_recipeItems[var2][0];
			Item var4 = (Item*)m_recipeItems[var2][1];
			var1->addRecipe(ItemStack(var3), NULL /* PORT: array init {"###", "###", "###", '#', var4} */);
			var1->addRecipe(ItemStack(var4, 9), NULL /* PORT: array init {"#", '#', var3} */);
		}

}
