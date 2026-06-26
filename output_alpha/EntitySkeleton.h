#ifndef ENTITYSKELETON_H
#define ENTITYSKELETON_H

// Auto-generated from EntitySkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"
#include <string>

class Entity;
class ItemStack;
class NBTTagCompound;
class World;

class EntitySkeleton : public EntityMobs {
public:
	EntitySkeleton();
	EntitySkeleton(World* var1);
	virtual ~EntitySkeleton();

	virtual void onLivingUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual ItemStack* getHeldItem();

public: // was protected
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual void attackEntity(Entity* var1, float var2);
	virtual int getDropItemId();

public: // was private
	static ItemStack* s_defaultHeldItem;

};

#endif // ENTITYSKELETON_H