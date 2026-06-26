#ifndef RECIPESARMOR_H
#define RECIPESARMOR_H

// Auto-generated from RecipesArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class CraftingManager;

class RecipesArmor {
public:
	RecipesArmor();
	virtual ~RecipesArmor();

	virtual void addRecipes(CraftingManager* var1);

private:
	std::string* m_recipePatterns;
	void* /* PORT: was Object */** m_recipeItems;

};

#endif // RECIPESARMOR_H