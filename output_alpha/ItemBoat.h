#ifndef ITEMBOAT_H
#define ITEMBOAT_H

// Auto-generated from ItemBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemBoat : public Item {
public:
	ItemBoat();
	ItemBoat(int var1);
	virtual ~ItemBoat();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

};

#endif // ITEMBOAT_H