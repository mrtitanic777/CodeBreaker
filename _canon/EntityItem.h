#ifndef ENTITYITEM_H
#define ENTITYITEM_H

// Auto-generated from EntityItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class EntityPlayer;
class ItemStack;
class NBTTagCompound;
class World;

class EntityItem : public Entity {
public:
	EntityItem(World* var1, double var2, double var4, double var6, ItemStack* var8);
	EntityItem(World* var1);
	virtual ~EntityItem();

	ItemStack* item;
	int bobTimer;
	int age;
	int delayBeforeCanPickup;
	int health;
	float bobOffset;

	virtual void onUpdate();
	virtual bool handleWaterMovement();
	virtual bool pushOutOfBlock(double var1, double var3, double var5);
	virtual void dealFireDamage(int var1);
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);

	static void initStatics();
};

#endif // ENTITYITEM_H