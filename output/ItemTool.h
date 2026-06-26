#ifndef ITEMTOOL_H
#define ITEMTOOL_H

// Auto-generated from ItemTool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class Block;
class ItemStack;

class ItemTool : public Item {
public:
	ItemTool();
	ItemTool(int var1, int var2, int var3, Block** var4);
	virtual ~ItemTool();

	virtual float getStrVsBlock(Block* var1);
	virtual void hitEntity(ItemStack* var1);
	virtual void onBlockDestroyed(ItemStack* var1);
	virtual int getDamageVsEntity();

public: // was private
	Block** m_blocksEffectiveAgainst;
	float m_efficiencyOnProperMaterial;
	int m_damageVsEntity;

public:  // array-length companions
	int m_blocksEffectiveAgainst_count;

};

#endif // ITEMTOOL_H