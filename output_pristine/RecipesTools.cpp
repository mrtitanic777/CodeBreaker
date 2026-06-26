#include "stdafx.h"
// Auto-generated from RecipesTools.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipesTools.h"
#include "Common.h"
#include "CraftingManager.h"
#include <string>
#include "Item.h"
#include "ItemStack.h"

RecipesTools::RecipesTools()
	: m_recipePatterns(new std::string[][])
	, m_recipeItems(new void* /* PORT: was Object */[][])
{
}

RecipesTools::~RecipesTools() {
	delete[] m_recipePatterns;
	m_recipePatterns = NULL;
	delete[] m_recipeItems;
	m_recipeItems = NULL;
}

void RecipesTools::addRecipes(CraftingManager* var1) {
		for(int var2 = 0; var2 < m_recipeItems[0] /* PORT: array length - use tracked size */; ++var2) {
			void* var3 = m_recipeItems[0][var2];

			for(int var4 = 0; var4 < m_recipeItems_count /* PORT: set this from allocation */ - 1; ++var4) {
				Item var5 = (Item)m_recipeItems[var4 + 1][var2];
				var1->addRecipe(new ItemStack(var5), new void*[]{m_recipePatterns[var4], '#', Item::s_bowlEmpty, 'X', var3});
			}
		}

}
