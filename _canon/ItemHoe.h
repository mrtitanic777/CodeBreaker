#ifndef ITEMHOE_H
#define ITEMHOE_H

// Auto-generated from ItemHoe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemHoe : public Item {
public:
	ItemHoe(int var1, int var2);
	virtual ~ItemHoe();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);
	virtual bool isFull3D();

	static void initStatics();
};

#endif // ITEMHOE_H