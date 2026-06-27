#ifndef ITEMBOW_H
#define ITEMBOW_H

// Auto-generated from ItemBow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemBow : public Item {
public:
	ItemBow(int var1);
	virtual ~ItemBow();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMBOW_H