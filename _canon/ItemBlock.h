#ifndef ITEMBLOCK_H
#define ITEMBLOCK_H

// Auto-generated from ItemBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemBlock : public Item {
public:
	ItemBlock(int var1);
	virtual ~ItemBlock();

	int blockID;

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

	static void initStatics();
};

#endif // ITEMBLOCK_H