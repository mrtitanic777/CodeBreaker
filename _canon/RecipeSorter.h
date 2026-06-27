#ifndef RECIPESORTER_H
#define RECIPESORTER_H

// Auto-generated from RecipeSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"
#include "Comparator.h"

class CraftingManager;
class CraftingRecipe;

class RecipeSorter : public Comparator {
public:
	RecipeSorter(CraftingManager* var1);
	virtual ~RecipeSorter();

	CraftingManager* craftingManager;

	virtual int a(CraftingRecipe* var1, CraftingRecipe* var2);
	virtual int compare(JObject var1, JObject var2);

	static void initStatics();
};

#endif // RECIPESORTER_H