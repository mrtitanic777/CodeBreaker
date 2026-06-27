#ifndef ENTITYSHEEP_H
#define ENTITYSHEEP_H

// Auto-generated from EntitySheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityAnimals.h"

class Entity;
class NBTTagCompound;
class World;

class EntitySheep : public EntityAnimals {
public:
	EntitySheep(World* var1);
	virtual ~EntitySheep();

	bool sheared;

	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();

	static void initStatics();
};

#endif // ENTITYSHEEP_H