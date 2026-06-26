#ifndef CRAFTINGRECIPE_H
#define CRAFTINGRECIPE_H

// Auto-generated from CraftingRecipe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemStack.h"

class CraftingRecipe {
public:
	CraftingRecipe();
	CraftingRecipe(int var1, int var2, int* var3, const ItemStack& var4);
	virtual ~CraftingRecipe();

	virtual bool matchRecipe(int* var1);
	virtual ItemStack createResult();
	virtual int b();

private:
	int width;
	int height;
	int* m_ingredientMap;
	ItemStack m_recipeOutput;
	bool m_hasRecipeOutput;

	bool matches(int* var1, int var2, int var3, bool var4);

};

#endif // CRAFTINGRECIPE_H