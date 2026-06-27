#ifndef ITEMBUCKET_H
#define ITEMBUCKET_H

// Auto-generated from ItemBucket.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemBucket : public Item {
public:
	ItemBucket(int var1, int var2);
	virtual ~ItemBucket();

	int isFull;

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMBUCKET_H