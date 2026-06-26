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

public: // was private
	static CraftingManager* instance;
	std::vector<void*> /* PORT: determine element type */ m_recipes;

	void addRecipe(ItemStack* var1, void* var2, int var2_count = 0);

};

#endif // CRAFTINGMANAGER_H