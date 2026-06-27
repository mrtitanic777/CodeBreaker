#ifndef ITEMARMOR_H
#define ITEMARMOR_H

// Auto-generated from ItemArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class ItemArmor : public Item {
public:
	ItemArmor(int var1, int var2, int var3, int var4);
	virtual ~ItemArmor();

	int armorLevel;
	int armorType;
	int damageReduceAmmount;
	int renderIndex;
	static JArray<int > s_damageReduceAmmountArray;
	static JArray<int > s_maxDamageArray;

	static void initStatics();
};

#endif // ITEMARMOR_H