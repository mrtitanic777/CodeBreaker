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

public:  // array-length companions
	int m_recipePatterns_count;
	int m_recipeItems_count;

};

#endif // RECIPESTOOLS_H