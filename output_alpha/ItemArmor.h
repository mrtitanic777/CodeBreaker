#ifndef ITEMARMOR_H
#define ITEMARMOR_H

// Auto-generated from ItemArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class ItemArmor : public Item {
public:
	ItemArmor();
	ItemArmor(int var1, int var2, int var3, int var4);
	virtual ~ItemArmor();

	int m_armorLevel;
	int m_armorType;
	int m_damageReduceAmmount;
	int m_renderIndex;


public: // was private
	static int* s_damageReduceAmmountArray;
	static int* s_maxDamageArray;

};

#endif // ITEMARMOR_H