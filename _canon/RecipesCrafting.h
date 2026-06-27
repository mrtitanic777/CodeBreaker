#ifndef RECIPESCRAFTING_H
#define RECIPESCRAFTING_H

// Auto-generated from RecipesCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class CraftingManager;

class RecipesCrafting {
public:
	virtual ~RecipesCrafting();

	virtual void addRecipes(CraftingManager* var1);

	static void initStatics();
};

#endif // RECIPESCRAFTING_H