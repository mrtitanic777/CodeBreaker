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

	virtual void func_1122_a(CraftingManager* var1);

public: // was private
	std::string** m_field_1665_a;
	void* /* PORT: was Object */** m_field_1664_b;

public:  // array-length companions
	int m_field_1665_a_count;
	int m_field_1664_b_count;

};

#endif // RECIPESTOOLS_H