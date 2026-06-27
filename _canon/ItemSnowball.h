#ifndef ITEMSNOWBALL_H
#define ITEMSNOWBALL_H

// Auto-generated from ItemSnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemSnowball : public Item {
public:
	ItemSnowball(int var1);
	virtual ~ItemSnowball();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMSNOWBALL_H