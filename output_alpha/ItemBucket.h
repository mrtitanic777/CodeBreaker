#ifndef ITEMBUCKET_H
#define ITEMBUCKET_H

// Auto-generated from ItemBucket.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemBucket : public Item {
public:
	ItemBucket();
	ItemBucket(int var1, int var2);
	virtual ~ItemBucket();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

public: // was private
	int m_isFull;

};

#endif // ITEMBUCKET_H