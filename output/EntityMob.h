#ifndef ENTITYMOB_H
#define ENTITYMOB_H

// Auto-generated from EntityMob.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntityMob : public EntityCreature {
public:
	EntityMob();
	EntityMob(World* var1);
	virtual ~EntityMob();

	virtual void onLivingUpdate();
	virtual void onEntityUpdate();
	virtual bool attackEntityFrom(Entity* var1, int var2);
	virtual bool getCanSpawnHere(float var1, float var2, float var3);

public: // was protected
	int attackStrength;

	virtual Entity* findPlayerToAttack();
	virtual void attackEntity(Entity* var1, float var2);
	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

};

#endif // ENTITYMOB_H