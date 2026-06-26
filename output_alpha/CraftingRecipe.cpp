#include "stdafx.h"
// Auto-generated from CraftingRecipe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingRecipe.h"
#include "Common.h"
#include "ItemStack.h"

CraftingRecipe::CraftingRecipe()
	: width(0)
	, height(0)
	, m_ingredientMap(0)
	, m_resultStack(NULL)
	, m_resultId(0)
{
}

CraftingRecipe::CraftingRecipe(int var1, int var2, int* var3, ItemStack* var4)
	: width(0)
	, height(0)
	, m_ingredientMap(0)
	, m_resultStack(NULL)
	, m_resultId(0)
{
		m_resultId = var4->itemID;
		width = var1;
		height = var2;
		m_ingredientMap = var3;
		m_resultStack = var4;
}

CraftingRecipe::~CraftingRecipe() {
	delete[] m_ingredientMap;
	m_ingredientMap = NULL;
	m_resultStack = NULL;
}

bool CraftingRecipe::matchRecipe(int* var1) {
		for(int var2 = 0; var2 <= 3 - width; ++var2) {
			for(int var3 = 0; var3 <= 3 - height; ++var3) {
				if(matches(var1, var2, var3, true)) {
					return true;
				}

				if(matches(var1, var2, var3, false)) {
					return true;
				}
			}
		}

		return false;
}

bool CraftingRecipe::matches(int* var1, int var2, int var3, bool var4) {
		for(int var5 = 0; var5 < 3; ++var5) {
			for(int var6 = 0; var6 < 3; ++var6) {
				int var7 = var5 - var2;
				int var8 = var6 - var3;
				int var9 = -1;
				if(var7 >= 0 && var8 >= 0 && var7 < width && var8 < height) {
					if(var4) {
						var9 = m_ingredientMap[width - var7 - 1 + var8 * width];
					} else {
						var9 = m_ingredientMap[var7 + var8 * width];
					}
				}

				if(var1[var5 + var6 * 3] != var9) {
					return false;
				}
			}
		}

		return true;
}

ItemStack* CraftingRecipe::createResult(int* var1) {
		return new ItemStack(m_resultStack->itemID, m_resultStack->stackSize);
}

int CraftingRecipe::getRecipeSize() {
		return width * height;
}
