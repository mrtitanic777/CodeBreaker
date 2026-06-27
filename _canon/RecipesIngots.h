#ifndef RECIPESINGOTS_H
#define RECIPESINGOTS_H

// Auto-generated from RecipesIngots.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class CraftingManager;

class RecipesIngots {
public:
	RecipesIngots();
	virtual ~RecipesIngots();

	JArray<JArray<JObject > > recipeItems;

	virtual void addRecipes(CraftingManager* var1);

	static void initStatics();
};

#endif // RECIPESINGOTS_H