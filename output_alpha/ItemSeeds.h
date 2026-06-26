#ifndef ITEMSEEDS_H
#define ITEMSEEDS_H

// Auto-generated from ItemSeeds.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemSeeds : public Item {
public:
	ItemSeeds();
	ItemSeeds(int var1, int var2);
	virtual ~ItemSeeds();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

public: // was private
	int m_field_318_a;

};

#endif // ITEMSEEDS_H