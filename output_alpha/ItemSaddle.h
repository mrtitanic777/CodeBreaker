#ifndef ITEMSADDLE_H
#define ITEMSADDLE_H

// Auto-generated from ItemSaddle.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityLiving;
class ItemStack;

class ItemSaddle : public Item {
public:
	ItemSaddle();
	ItemSaddle(int var1);
	virtual ~ItemSaddle();

	virtual void func_4019_b(ItemStack* var1, EntityLiving* var2);
	virtual void hitEntity(ItemStack* var1, EntityLiving* var2);

};

#endif // ITEMSADDLE_H