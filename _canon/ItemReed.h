#ifndef ITEMREED_H
#define ITEMREED_H

// Auto-generated from ItemReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class Block;
class EntityPlayer;
class ItemStack;
class World;

class ItemReed : public Item {
public:
	ItemReed(int var1, Block* var2);
	virtual ~ItemReed();

	int blockID;

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

	static void initStatics();
};

#endif // ITEMREED_H