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

	virtual void func_766_a(CraftingManager* var1);

public: // was private
	std::string** m_field_1100_a;
	void* /* PORT: was Object */** m_field_1099_b;

public:  // array-length companions
	int m_field_1100_a_count;
	int m_field_1099_b_count;

};

#endif // RECIPESWEAPONS_H