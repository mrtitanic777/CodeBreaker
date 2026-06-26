#ifndef ENTITYITEM_H
#define ENTITYITEM_H

// Auto-generated from EntityItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include "ItemStack.h"
#include <string>

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityItem : public Entity {
public:
	EntityItem();
	EntityItem(World* var1, float var2, float var3, float var4, const ItemStack& var5);
	EntityItem(World* var1);
	virtual ~EntityItem();

	ItemStack m_item;
	bool m_hasItem;
	int m_age;
	int m_delayBeforeCanPickup;
	float m_hoverStart;

	virtual void onEntityUpdate();
	virtual bool attackEntityFrom(Entity* var1, int var2);
	virtual void onCollideWithPlayer(EntityPlayer* var1);

protected:
	virtual void dealFireDamage(int var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

private:
	int m_unknownEntityItemInt;
	int health;

};

#endif // ENTITYITEM_H