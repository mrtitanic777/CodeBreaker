#ifndef ITEMFLINTANDSTEEL_H
#define ITEMFLINTANDSTEEL_H

// Auto-generated from ItemFlintAndSteel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class ItemStack;
class World;

class ItemFlintAndSteel : public Item {
public:
	ItemFlintAndSteel();
	ItemFlintAndSteel(int var1);
	virtual ~ItemFlintAndSteel();

	virtual bool onItemUse(const ItemStack& var1, World* var2, int var3, int var4, int var5, int var6);

};

#endif // ITEMFLINTANDSTEEL_H