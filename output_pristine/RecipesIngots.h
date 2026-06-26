#ifndef RECIPESINGOTS_H
#define RECIPESINGOTS_H

// Auto-generated from RecipesIngots.java
// Target: Xbox 360 / C++03 / DirectX 9

class CraftingManager;

class RecipesIngots {
public:
	RecipesIngots();
	virtual ~RecipesIngots();

	virtual void addRecipes(CraftingManager* var1);

private:
	void* /* PORT: was Object */** m_recipeItems;

public:  // array-length companions
	int m_recipeItems_count;

};

#endif // RECIPESINGOTS_H