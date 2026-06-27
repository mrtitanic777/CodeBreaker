#ifndef CRAFTINGRECIPE_H
#define CRAFTINGRECIPE_H

// Auto-generated from CraftingRecipe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ItemStack;

class CraftingRecipe {
public:
	CraftingRecipe(int var1, int var2, JArray<int > var3, ItemStack* var4);
	virtual ~CraftingRecipe();

	int width;
	int height;
	JArray<int > ingredientMap;
	ItemStack* resultStack;
	int resultId;

	virtual bool matchRecipe(JArray<int > var1);
	virtual bool matches(JArray<int > var1, int var2, int var3, bool var4);
	virtual ItemStack* createResult(JArray<int > var1);
	virtual int getRecipeSize();

	static void initStatics();
};

#endif // CRAFTINGRECIPE_H