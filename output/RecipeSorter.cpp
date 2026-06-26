#include "stdafx.h"
// Auto-generated from RecipeSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipeSorter.h"
#include "Common.h"
#include "CraftingManager.h"
#include "CraftingRecipe.h"

RecipeSorter::RecipeSorter()
{
}

RecipeSorter::RecipeSorter(CraftingManager* var1)
{
}

RecipeSorter::~RecipeSorter() {
	// No dynamic resources
}

int RecipeSorter::compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2) {
		CraftingRecipe* var10000 = (CraftingRecipe*)var1;
		CraftingRecipe* var4 = (CraftingRecipe*)var2;
		CraftingRecipe* var3 = var10000;
		return var4->b() < var3->b() ? -1 : (var4->b() > var3->b() ? 1 : 0);
}
