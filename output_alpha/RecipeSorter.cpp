#include "stdafx.h"
// Auto-generated from RecipeSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RecipeSorter.h"
#include "Common.h"
#include "CraftingManager.h"
#include "CraftingRecipe.h"

RecipeSorter::RecipeSorter()
	: m_field_1557_a(NULL)
{
}

RecipeSorter::RecipeSorter(CraftingManager* var1)
	: m_field_1557_a(NULL)
{
		m_field_1557_a = var1;
}

RecipeSorter::~RecipeSorter() {
	m_field_1557_a = NULL;
}

int RecipeSorter::a(CraftingRecipe* var1, CraftingRecipe* var2) {
		return var2->getRecipeSize() < var1->getRecipeSize() ? -1 : (var2->getRecipeSize() > var1->getRecipeSize() ? 1 : 0);
}

int RecipeSorter::compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2) {
		return a((CraftingRecipe*)var1, (CraftingRecipe*)var2);
}
