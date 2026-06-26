#ifndef ITEMSEEDS_H
#define ITEMSEEDS_H

// Auto-generated from ItemSeeds.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class ItemStack;
class World;

class ItemSeeds : public Item {
public:
	ItemSeeds();
	ItemSeeds(int var1, int var2);
	virtual ~ItemSeeds();

	virtual bool onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6);

public: // was private
	int m_blockType;

};

#endif // ITEMSEEDS_H