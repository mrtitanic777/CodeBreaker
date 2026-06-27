#ifndef RECIPESARMOR_H
#define RECIPESARMOR_H

// Auto-generated from RecipesArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class CraftingManager;

class RecipesArmor {
public:
	RecipesArmor();
	virtual ~RecipesArmor();

	JArray<JArray<JavaString > > craftingPatterns;
	JArray<JArray<JObject > > armorItems;

	virtual void addRecipes(CraftingManager* var1);

	static void initStatics();
};

#endif // RECIPESARMOR_H