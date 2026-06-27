#ifndef RECIPESFOOD_H
#define RECIPESFOOD_H

// Auto-generated from RecipesFood.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class CraftingManager;

class RecipesFood {
public:
	virtual ~RecipesFood();

	virtual void addRecipes(CraftingManager* var1);

	static void initStatics();
};

#endif // RECIPESFOOD_H