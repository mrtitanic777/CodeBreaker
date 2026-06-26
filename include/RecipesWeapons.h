#ifndef RECIPESWEAPONS_H
#define RECIPESWEAPONS_H

// Auto-generated from RecipesWeapons.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class CraftingManager;

class RecipesWeapons {
public:
	RecipesWeapons();
	virtual ~RecipesWeapons();

	virtual void addRecipes(CraftingManager* var1);

private:
	std::string* m_recipePatterns;
	void* /* PORT: was Object */** m_recipeItems;

};

#endif // RECIPESWEAPONS_H