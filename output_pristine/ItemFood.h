#ifndef ITEMFOOD_H
#define ITEMFOOD_H

// Auto-generated from ItemFood.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemFood : public Item {
public:
	ItemFood();
	ItemFood(int var1, int var2);
	virtual ~ItemFood();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

private:
	int m_healAmount;

};

#endif // ITEMFOOD_H