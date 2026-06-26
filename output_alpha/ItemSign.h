#ifndef ITEMSIGN_H
#define ITEMSIGN_H

// Auto-generated from ItemSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemSign : public Item {
public:
	ItemSign();
	ItemSign(int var1);
	virtual ~ItemSign();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

};

#endif // ITEMSIGN_H