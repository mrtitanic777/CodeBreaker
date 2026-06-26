#ifndef RECIPESTOOLS_H
#define RECIPESTOOLS_H

// Auto-generated from RecipesTools.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class CraftingManager;

class RecipesTools {
public:
	RecipesTools();
	virtual ~RecipesTools();

	virtual void addRecipes(CraftingManager* var1);

private:
	std::string* m_recipePatterns;
	void* /* PORT: was Object */** m_recipeItems;

};

#endif // RECIPESTOOLS_H