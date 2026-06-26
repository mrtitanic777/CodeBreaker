#ifndef ITEMFISHINGROD_H
#define ITEMFISHINGROD_H

// Auto-generated from ItemFishingRod.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemFishingRod : public Item {
public:
	ItemFishingRod();
	ItemFishingRod(int var1);
	virtual ~ItemFishingRod();

	virtual bool isFull3D();
	virtual bool shouldRotateAroundWhenRendering();
	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

};

#endif // ITEMFISHINGROD_H