#ifndef ENTITYPIGZOMBIE_H
#define ENTITYPIGZOMBIE_H

// Auto-generated from EntityPigZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityZombie.h"

class Entity;
class ItemStack;
class NBTTagCompound;
class World;

class EntityPigZombie : public EntityZombie {
public:
	EntityPigZombie(World* var1);
	virtual ~EntityPigZombie();

	int angerLevel;
	int randomSoundDelay;
	static ItemStack* s_defaultHeldItem;

	virtual void onUpdate();
	virtual bool getCanSpawnHere();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual Entity* findPlayerToAttack();
	virtual void onLivingUpdate();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void becomeAngryAt(Entity* var1);
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual int getDropItemId();
	virtual ItemStack* getHeldItem();

	static void initStatics();
};

#endif // ENTITYPIGZOMBIE_H