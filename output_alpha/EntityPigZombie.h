#ifndef ENTITYPIGZOMBIE_H
#define ENTITYPIGZOMBIE_H

// Auto-generated from EntityPigZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityZombie.h"
#include <string>

class Entity;
class ItemStack;
class NBTTagCompound;
class World;

class EntityPigZombie : public EntityZombie {
public:
	EntityPigZombie();
	EntityPigZombie(World* var1);
	virtual ~EntityPigZombie();

	virtual void onUpdate();
	virtual bool getCanSpawnHere();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onLivingUpdate();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual ItemStack* getHeldItem();

public: // was protected
	virtual Entity* findPlayerToAttack();
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();

public: // was private
	int m_angerLevel;
	int m_randomSoundDelay;
	static ItemStack* s_defaultHeldItem;

	void becomeAngryAt(Entity* var1);

};

#endif // ENTITYPIGZOMBIE_H