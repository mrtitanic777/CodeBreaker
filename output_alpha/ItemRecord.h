#ifndef ITEMRECORD_H
#define ITEMRECORD_H

// Auto-generated from ItemRecord.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"
#include <string>

class EntityPlayer;
class ItemStack;
class World;

class ItemRecord : public Item {
public:
	ItemRecord();
	ItemRecord(int var1, const std::string& var2);
	virtual ~ItemRecord();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

public: // was protected
public: // was private
	std::string m_recordName;

};

#endif // ITEMRECORD_H