#ifndef ENTITYMOBS_H
#define ENTITYMOBS_H

// Auto-generated from EntityMobs.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityCreature.h"
#include "IMobs.h"

class Entity;
class NBTTagCompound;
class World;

class EntityMobs : public EntityCreature, public IMobs {
public:
	EntityMobs(World* var1);
	virtual ~EntityMobs();

	int attackStrength;

	virtual void onLivingUpdate();
	virtual void onUpdate();
	virtual Entity* findPlayerToAttack();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void attackEntity(Entity* var1, float var2);
	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool getCanSpawnHere();

	static void initStatics();
};

#endif // ENTITYMOBS_H