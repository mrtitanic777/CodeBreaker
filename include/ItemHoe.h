#ifndef ITEMHOE_H
#define ITEMHOE_H

// Auto-generated from ItemHoe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class ItemStack;
class World;

class ItemHoe : public Item {
public:
	ItemHoe();
	ItemHoe(int var1, int var2);
	virtual ~ItemHoe();

	virtual bool onItemUse(const ItemStack& var1, World* var2, int var3, int var4, int var5, int var6);

};

#endif // ITEMHOE_H