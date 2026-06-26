#ifndef ITEMREED_H
#define ITEMREED_H

// Auto-generated from ItemReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class Block;
class EntityPlayer;
class ItemStack;
class World;

class ItemReed : public Item {
public:
	ItemReed();
	ItemReed(int var1, Block* var2);
	virtual ~ItemReed();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

public: // was private
	int m_field_320_a;

};

#endif // ITEMREED_H