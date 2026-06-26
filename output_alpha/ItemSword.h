#ifndef ITEMSWORD_H
#define ITEMSWORD_H

// Auto-generated from ItemSword.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class Block;
class Entity;
class EntityLiving;
class ItemStack;

class ItemSword : public Item {
public:
	ItemSword();
	ItemSword(int var1, int var2);
	virtual ~ItemSword();

	virtual float getStrVsBlock(ItemStack* var1, Block* var2);
	virtual void hitEntity(ItemStack* var1, EntityLiving* var2);
	virtual void hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5);
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool isFull3D();

public: // was private
	int m_weaponDamage;

};

#endif // ITEMSWORD_H