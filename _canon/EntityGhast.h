#ifndef ENTITYGHAST_H
#define ENTITYGHAST_H

// Auto-generated from EntityGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFlying.h"
#include "IMobs.h"

class Entity;
class World;

class EntityGhast : public EntityFlying, public IMobs {
public:
	EntityGhast(World* var1);
	virtual ~EntityGhast();

	int courseChangeCooldown;
	double waypointX;
	double waypointY;
	double waypointZ;
	Entity* targetedEntity;
	int aggroCooldown;
	int prevAttackCounter;
	int attackCounter;

	virtual void updateEntityActionState();
	virtual bool isCourseTraversable(double var1, double var3, double var5, double var7);
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual int getDropItemId();
	virtual float getSoundVolume();
	virtual bool getCanSpawnHere();
	virtual int getMaxSpawnedInChunk();

	static void initStatics();
};

#endif // ENTITYGHAST_H