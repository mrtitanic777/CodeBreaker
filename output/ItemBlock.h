#ifndef ITEMBLOCK_H
#define ITEMBLOCK_H

// Auto-generated from ItemBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class ItemStack;
class World;

class ItemBlock : public Item {
public:
	ItemBlock();
	ItemBlock(int var1);
	virtual ~ItemBlock();

	virtual bool onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6);
	virtual bool onPlaced(World* var1, float var2, float var3, float var4);

public: // was private
	int blockID;

};

#endif // ITEMBLOCK_H