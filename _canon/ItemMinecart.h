#ifndef ITEMMINECART_H
#define ITEMMINECART_H

// Auto-generated from ItemMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemMinecart : public Item {
public:
	ItemMinecart(int var1, int var2);
	virtual ~ItemMinecart();

	int minecartType;

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

	static void initStatics();
};

#endif // ITEMMINECART_H