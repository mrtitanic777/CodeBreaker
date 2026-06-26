#include "stdafx.h"
// Auto-generated from CraftingManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingManager.h"
#include "Common.h"
#include "ItemStack.h"
#include <string>
#include "Block.h"
#include "CraftingRecipe.h"
#include "Item.h"
#include "RecipeSorter.h"
#include "RecipesArmor.h"
#include "RecipesCrafting.h"
#include "RecipesFood.h"
#include "RecipesIngots.h"
#include "RecipesTools.h"
#include "RecipesWeapons.h"

CraftingManager* CraftingManager::instance = new CraftingManager();

CraftingManager::CraftingManager()
{
		(new RecipesTools())->addRecipes(this);
		(new RecipesWeapons())->addRecipes(this);
		(new RecipesIngots())->addRecipes(this);
		new RecipesFood();
		addRecipe(ItemStack(Item::s_swordGold), NULL /* PORT: array init {"Y", "X", "#", 'X', Block::s_mushroomBrown, 'Y', Block::s_mushroomRed, '#', Item::s_bowlSoup} */);
		addRecipe(ItemStack(Item::s_swordGold), NULL /* PORT: array init {"Y", "X", "#", 'X', Block::s_mushroomRed, 'Y', Block::s_mushroomBrown, '#', Item::s_bowlSoup} */);
		new RecipesCrafting();
		addRecipe(ItemStack(Block::s_crate), NULL /* PORT: array init {"###", "# #", "###", '#', Block::s_planks} */);
		addRecipe(ItemStack(Block::s_workbench), NULL /* PORT: array init {"##", "##", '#', Block::s_planks} */);
		(new RecipesArmor())->addRecipes(this);
		addRecipe(ItemStack(Block::s_clothGray, 1), NULL /* PORT: array init {"###", "###", "###", '#', Item::s_feather} */);
		addRecipe(ItemStack(Block::s_tnt, 1), NULL /* PORT: array init {"X#X", "#X#", "X#X", 'X', Item::s_hoeWood, '#', Block::s_sand} */);
		addRecipe(ItemStack(Block::s_stairSingle, 3), NULL /* PORT: array init {"###", '#', Block::s_cobblestone} */);
		addRecipe(ItemStack(Item::s_bowlEmpty, 4), NULL /* PORT: array init {"#", "#", '#', Block::s_planks} */);
		addRecipe(ItemStack(Block::s_torch, 4), NULL /* PORT: array init {"X", "#", 'X', Item::s_coal, '#', Item::s_bowlEmpty} */);
		addRecipe(ItemStack(Item::s_bowlSoup, 4), NULL /* PORT: array init {"# #", " # ", '#', Block::s_planks} */);
		addRecipe(ItemStack(Item::s_helmetLeather, 1), NULL /* PORT: array init {"###", "###", '#', Item::s_bread} */);
		Collections::sort(m_recipes, new RecipeSorter(this));
		DbgPrintF("%s\n", m_recipes.size() + " m_recipes");
}

CraftingManager::~CraftingManager() {
	// No dynamic resources
}

CraftingManager* CraftingManager::getInstance() {
		return instance;
}

void CraftingManager::addRecipe(const ItemStack& var1, Object* var2, int var2_count) {
		std::string var3 = "";
		int var4 = 0;
		int var5 = 0;
		int var6 = 0;
		// PORT-BLOCKED: if(var2[0] instanceof std::string[]) {
			++var4;
			std::string[] var11 = (std::string[])var2[0];

			for(int var8 = 0; var8 < var11_count /* PORT: set this from allocation */; ++var8) {
				std::string var9 = var11[var8];
				++var6;
				var5 = var9.length();
				var3 = var3 + var9;
			}
		} else {
			// PORT-BLOCKED: while(var2[var4] instanceof std::string) {
				std::string var7 = (std::string)var2[var4++];
				++var6;
				var5 = var7.length();
				var3 = var3 + var7;
			}
		}

		HashMap var12;
		int var15;
		for(var12 = new HashMap(); var4 < var2_count /* PORT: set this from allocation */; var4 += 2) {
			char var13 = (char)var2[var4];
			var15 = 0;
			// PORT-BLOCKED: if(var2[var4 + 1] instanceof Item) {
				var15 = ((Item*)var2[var4 + 1]).shiftedIndex;
			// PORT-BLOCKED: } else if(var2[var4 + 1] instanceof Block) {
				var15 = ((Block*)var2[var4 + 1]).blockID;
			}

   var12[0] = var13, var15;
		}

		int* var14 = new int[var5 * var6];

		for(var15 = 0; var15 < var5 * var6; ++var15) {
			char var10 = var3.charAt(var15);
			if(var12.containsKey(var10)) {
				var14[var15] = ((int)var12.get(var10)).intValue();
			} else {
				var14[var15] = -1;
			}
		}

		m_recipes.push_back(new CraftingRecipe(var5, var6, var14, var1));
}

ItemStack CraftingManager::findMatchingRecipe(int* var1) {
		for(size_t var2 = 0; var2 < m_recipes.size(); ++var2) {
			CraftingRecipe var3 = (CraftingRecipe*)m_recipes[var2];
			if(var3.matchRecipe(var1)) {
				return var3.createResult();
			}
		}

		return NULL;
}
