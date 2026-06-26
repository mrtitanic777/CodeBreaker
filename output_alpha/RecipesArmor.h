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

	virtual void func_1148_a(CraftingManager* var1);

public: // was private
	std::string** m_field_1681_a;
	void* /* PORT: was Object */** m_field_1680_b;

public:  // array-length companions
	int m_field_1681_a_count;
	int m_field_1680_b_count;

};

#endif // RECIPESARMOR_H