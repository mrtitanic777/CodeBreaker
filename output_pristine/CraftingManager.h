#ifndef CRAFTINGMANAGER_H
#define CRAFTINGMANAGER_H

// Auto-generated from CraftingManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

class ItemStack;

class CraftingManager {
public:
	CraftingManager();
	virtual ~CraftingManager();

	static CraftingManager* getInstance();
	virtual ItemStack* findMatchingRecipe(int* var1);

private:
	static CraftingManager* instance;
	std::vector<void*> /* PORT: determine element type */ m_recipes;

	void addRecipe(ItemStack* var1, Object* var2, int var2_count);

};

#endif // CRAFTINGMANAGER_H