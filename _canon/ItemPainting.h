#ifndef ITEMPAINTING_H
#define ITEMPAINTING_H

// Auto-generated from ItemPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemPainting : public Item {
public:
	ItemPainting(int var1);
	virtual ~ItemPainting();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

	static void initStatics();
};

#endif // ITEMPAINTING_H