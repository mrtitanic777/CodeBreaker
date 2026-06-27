#ifndef ENTITYSKELETON_H
#define ENTITYSKELETON_H

// Auto-generated from EntitySkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityMobs.h"

class Entity;
class ItemStack;
class NBTTagCompound;
class World;

class EntitySkeleton : public EntityMobs {
public:
	EntitySkeleton(World* var1);
	virtual ~EntitySkeleton();

	static ItemStack* s_defaultHeldItem;

	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual void onLivingUpdate();
	virtual void attackEntity(Entity* var1, float var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual int getDropItemId();
	virtual ItemStack* getHeldItem();

	static void initStatics();
};

#endif // ENTITYSKELETON_H