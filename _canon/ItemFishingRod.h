#ifndef ITEMFISHINGROD_H
#define ITEMFISHINGROD_H

// Auto-generated from ItemFishingRod.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemFishingRod : public Item {
public:
	ItemFishingRod(int var1);
	virtual ~ItemFishingRod();

	virtual bool isFull3D();
	virtual bool shouldRotateAroundWhenRendering();
	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMFISHINGROD_H