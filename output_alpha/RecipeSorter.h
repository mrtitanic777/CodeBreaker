#ifndef RECIPESORTER_H
#define RECIPESORTER_H

// Auto-generated from RecipeSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

class CraftingManager;
class CraftingRecipe;

class RecipeSorter {
public:
	RecipeSorter();
	RecipeSorter(CraftingManager* var1);
	virtual ~RecipeSorter();

	virtual int a(CraftingRecipe* var1, CraftingRecipe* var2);
	virtual int compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2);
	bool operator()(void* a, void* b) { return compare(a, b) < 0; }

public: // was private
	CraftingManager* m_field_1557_a;


};

#endif // RECIPESORTER_H