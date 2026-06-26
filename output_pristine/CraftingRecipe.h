#ifndef CRAFTINGRECIPE_H
#define CRAFTINGRECIPE_H

// Auto-generated from CraftingRecipe.java
// Target: Xbox 360 / C++03 / DirectX 9

class ItemStack;

class CraftingRecipe {
public:
	CraftingRecipe();
	CraftingRecipe(int var1, int var2, int* var3, ItemStack* var4);
	virtual ~CraftingRecipe();

	virtual bool matchRecipe(int* var1);
	virtual ItemStack* createResult();
	virtual int b();

private:
	int width;
	int height;
	int* m_ingredientMap;
	ItemStack* m_recipeOutput;

	bool matches(int* var1, int var2, int var3, bool var4);

public:  // array-length companions
	int m_ingredientMap_count;

};

#endif // CRAFTINGRECIPE_H