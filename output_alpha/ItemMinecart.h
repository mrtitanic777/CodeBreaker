#ifndef ITEMMINECART_H
#define ITEMMINECART_H

// Auto-generated from ItemMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemMinecart : public Item {
public:
	ItemMinecart();
	ItemMinecart(int var1, int var2);
	virtual ~ItemMinecart();

	int m_field_317_a;

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

};

#endif // ITEMMINECART_H