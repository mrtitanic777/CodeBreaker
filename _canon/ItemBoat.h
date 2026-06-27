#ifndef ITEMBOAT_H
#define ITEMBOAT_H

// Auto-generated from ItemBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemBoat : public Item {
public:
	ItemBoat(int var1);
	virtual ~ItemBoat();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMBOAT_H