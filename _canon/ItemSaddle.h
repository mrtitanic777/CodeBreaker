#ifndef ITEMSADDLE_H
#define ITEMSADDLE_H

// Auto-generated from ItemSaddle.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityLiving;
class ItemStack;

class ItemSaddle : public Item {
public:
	ItemSaddle(int var1);
	virtual ~ItemSaddle();

	virtual void useItemOnEntity(ItemStack* var1, EntityLiving* var2);
	virtual void hitEntity(ItemStack* var1, EntityLiving* var2);

	static void initStatics();
};

#endif // ITEMSADDLE_H