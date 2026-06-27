#ifndef ITEMRECORD_H
#define ITEMRECORD_H

// Auto-generated from ItemRecord.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Item.h"

class EntityPlayer;
class ItemStack;
class World;

class ItemRecord : public Item {
public:
	ItemRecord(int var1, const JavaString& var2);
	virtual ~ItemRecord();

	JavaString recordName;

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

	static void initStatics();
};

#endif // ITEMRECORD_H