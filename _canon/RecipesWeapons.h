#ifndef RECIPESWEAPONS_H
#define RECIPESWEAPONS_H

// Auto-generated from RecipesWeapons.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class CraftingManager;

class RecipesWeapons {
public:
	RecipesWeapons();
	virtual ~RecipesWeapons();

	JArray<JArray<JavaString > > recipePatterns;
	JArray<JArray<JObject > > recipeItems;

	virtual void addRecipes(CraftingManager* var1);

	static void initStatics();
};

#endif // RECIPESWEAPONS_H