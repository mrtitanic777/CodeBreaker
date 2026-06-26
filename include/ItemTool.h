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
	ItemTool(int var1, int var2, int var3, Block* var4);
	virtual ~ItemTool();

	virtual float getStrVsBlock(Block* var1);
	virtual void hitEntity(const ItemStack& var1);
	virtual void onBlockDestroyed(const ItemStack& var1);
	virtual int getDamageVsEntity();

private:
	Block* m_blocksEffectiveAgainst;
	float m_efficiencyOnProperMaterial;
	int m_damageVsEntity;

};

#endif // ITEMTOOL_H