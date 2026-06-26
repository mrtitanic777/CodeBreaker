#ifndef ITEMSWORD_H
#define ITEMSWORD_H

// Auto-generated from ItemSword.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class Block;
class ItemStack;

class ItemSword : public Item {
public:
	ItemSword();
	ItemSword(int var1, int var2);
	virtual ~ItemSword();

	virtual float getStrVsBlock(Block* var1);
	virtual void hitEntity(ItemStack* var1);
	virtual void onBlockDestroyed(ItemStack* var1);
	virtual int getDamageVsEntity();

private:
	int m_weaponDamage;

};

#endif // ITEMSWORD_H