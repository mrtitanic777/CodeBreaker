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

public:  // array-length companions
	int m_recipePatterns_count;
	int m_recipeItems_count;

};

#endif // RECIPESARMOR_H