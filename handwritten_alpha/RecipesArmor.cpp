#include "stdafx.h"
// Stub: Alpha's recipes are registered directly in CraftingManager (handwritten_alpha/CraftingManager.cpp);
// this RecipesArmor::addRecipes is a no-op so it doesn't double-register (and the converter's degenerate
// Java-varargs addRecipe stub is bypassed). The class stays so any caller still links.
#include "RecipesArmor.h"

RecipesArmor::RecipesArmor() {}
RecipesArmor::~RecipesArmor() {}
void RecipesArmor::func_1148_a(CraftingManager* var1) { (void)var1; }
