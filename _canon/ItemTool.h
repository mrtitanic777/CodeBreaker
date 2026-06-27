#ifndef ITEMTOOL_H
#define ITEMTOOL_H

// Auto-generated from ItemTool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class Block;
class Entity;
class EntityLiving;
class ItemStack;

class ItemTool : public Item {
public:
	ItemTool(int var1, int var2, int var3, JArray<Block* > var4);
	virtual ~ItemTool();

	JArray<Block* > blocksEffectiveAgainst;
	float efficiencyOnProperMaterial;
	int damageVsEntity;
	int ingredientQuality;

	virtual float getStrVsBlock(ItemStack* var1, Block* var2);
	virtual void hitEntity(ItemStack* var1, EntityLiving* var2);
	virtual void hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5);
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool isFull3D();

	static void initStatics();
};

#endif // ITEMTOOL_H