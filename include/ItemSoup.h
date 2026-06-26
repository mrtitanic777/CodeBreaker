#ifndef ITEMSOUP_H
#define ITEMSOUP_H

// Auto-generated from ItemSoup.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFood.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemSoup : public ItemFood {
public:
	ItemSoup();
	ItemSoup(int var1, int var2);
	virtual ~ItemSoup();

	virtual ItemStack onItemRightClick(const ItemStack& var1, World* var2, EntityPlayer* var3);

};

#endif // ITEMSOUP_H