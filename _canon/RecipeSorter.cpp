#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from RecipeSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipeSorter.h"
#include "Comparator.h"
#include "CraftingManager.h"
#include "CraftingRecipe.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

RecipeSorter::RecipeSorter(CraftingManager* var1)
	: craftingManager(NULL)
{
	this->craftingManager = var1;
}

RecipeSorter::~RecipeSorter()
{
}

int RecipeSorter::a(CraftingRecipe* var1, CraftingRecipe* var2)
{
	return var2->getRecipeSize() < var1->getRecipeSize() ? -1 : (var2->getRecipeSize() > var1->getRecipeSize() ? 1 : 0);
}

int RecipeSorter::compare(JObject var1, JObject var2)
{
	return this->a((CraftingRecipe*)var1, (CraftingRecipe*)var2);
}

void RecipeSorter::initStatics()
{
}
