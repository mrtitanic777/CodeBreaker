#ifndef ITEMTOOL_H
#define ITEMTOOL_H

// Auto-generated from ItemTool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class Block;
class Entity;
class EntityLiving;
class ItemStack;

class ItemTool : public Item {
public:
	ItemTool();
	ItemTool(int var1, int var2, int var3, Block** var4);
	virtual ~ItemTool();

	virtual float getStrVsBlock(ItemStack* var1, Block* var2);
	virtual void hitEntity(ItemStack* var1, EntityLiving* var2);
	virtual void hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5);
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool isFull3D();

public: // was protected
	int m_ingredientQuality;

public: // was private
	Block** m_blocksEffectiveAgainst;
	float m_efficiencyOnProperMaterial;
	int m_damageVsEntity;

public:  // array-length companions
	int m_blocksEffectiveAgainst_count;

};

#endif // ITEMTOOL_H