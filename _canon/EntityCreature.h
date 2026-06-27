#ifndef ENTITYCREATURE_H
#define ENTITYCREATURE_H

// Auto-generated from EntityCreature.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityLiving.h"

class Entity;
class PathEntity;
class World;

class EntityCreature : public EntityLiving {
public:
	EntityCreature(World* var1);
	virtual ~EntityCreature();

	PathEntity* pathToEntity;
	Entity* playerToAttack;
	bool hasAttacked;

	virtual void updateEntityActionState();
	virtual void attackEntity(Entity* var1, float var2);
	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual Entity* findPlayerToAttack();
	virtual bool getCanSpawnHere();

	static void initStatics();
};

#endif // ENTITYCREATURE_H