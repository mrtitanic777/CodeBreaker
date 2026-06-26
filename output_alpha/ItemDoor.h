#ifndef ITEMDOOR_H
#define ITEMDOOR_H

// Auto-generated from ItemDoor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Item.h"

class EntityPlayer;
class ItemStack;
class Material;
class World;

class ItemDoor : public Item {
public:
	ItemDoor();
	ItemDoor(int var1, Material* var2);
	virtual ~ItemDoor();

	virtual bool onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7);

public: // was private
	Material* m_field_321_a;

};

#endif // ITEMDOOR_H