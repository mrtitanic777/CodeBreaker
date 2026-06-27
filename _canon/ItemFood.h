#ifndef ITEMFOOD_H
#define ITEMFOOD_H

// Auto-generated from ItemFood.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemFood : public Item {
public:
	ItemFood(int var1, int var2);
	virtual ~ItemFood();

	int healAmount;

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMFOOD_H