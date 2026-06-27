#ifndef CRAFTINGMANAGER_H
#define CRAFTINGMANAGER_H

// Auto-generated from CraftingManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class ItemStack;

class CraftingManager {
public:
	CraftingManager();
	virtual ~CraftingManager();

	JList<void*> recipes;
	static CraftingManager* s_instance;

	static CraftingManager* getInstance();
	virtual void addRecipe(ItemStack* var1, JArray<JObject > var2);
	virtual ItemStack* craft(JArray<int > var1);

	static void initStatics();
};

#endif // CRAFTINGMANAGER_H