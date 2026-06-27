#ifndef ITEMSOUP_H
#define ITEMSOUP_H

// Auto-generated from ItemSoup.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "ItemFood.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemSoup : public ItemFood {
public:
	ItemSoup(int var1, int var2);
	virtual ~ItemSoup();

	virtual ItemStack* onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3);

	static void initStatics();
};

#endif // ITEMSOUP_H