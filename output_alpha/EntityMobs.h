#ifndef ENTITYMOBS_H
#define ENTITYMOBS_H

// Auto-generated from EntityMobs.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"
#include "IMobs.h"

class Entity;
class NBTTagCompound;
class World;

class EntityMobs : public EntityCreature, public IMobs {
public:
	EntityMobs();
	EntityMobs(World* var1);
	virtual ~EntityMobs();

	virtual void onLivingUpdate();
	virtual void onUpdate();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool getCanSpawnHere();

public: // was protected
	int m_field_762_e;

	virtual Entity* findPlayerToAttack();
	virtual void attackEntity(Entity* var1, float var2);
	virtual float func_439_a(int var1, int var2, int var3);

};

#endif // ENTITYMOBS_H