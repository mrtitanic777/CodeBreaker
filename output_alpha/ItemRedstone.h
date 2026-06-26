#ifndef ITEMREDSTONE_H
#define ITEMREDSTONE_H

// Auto-generated from ItemRedstone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemRedstone : public Item {
public:
	ItemRedstone();
	ItemRedstone(int var1);
	virtual ~ItemRedstone();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

};

#endif // ITEMREDSTONE_H