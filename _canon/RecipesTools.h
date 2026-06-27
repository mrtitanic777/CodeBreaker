#ifndef RECIPESTOOLS_H
#define RECIPESTOOLS_H

// Auto-generated from RecipesTools.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class CraftingManager;

class RecipesTools {
public:
	RecipesTools();
	virtual ~RecipesTools();

	JArray<JArray<JavaString > > recipePatterns;
	JArray<JArray<JObject > > recipeItems;

	virtual void addRecipes(CraftingManager* var1);

	static void initStatics();
};

#endif // RECIPESTOOLS_H