#ifndef ENTITYCREATURE_H
#define ENTITYCREATURE_H

// Auto-generated from EntityCreature.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"

class Entity;
class PathEntity;
class World;

class EntityCreature : public EntityLiving {
public:
	EntityCreature();
	EntityCreature(World* var1);
	virtual ~EntityCreature();

	virtual bool getCanSpawnHere(float var1, float var2, float var3);

protected:
	Entity* m_playerToAttack;
	bool m_hasAttacked;

	virtual void updatePlayerActionState();
	virtual void attackEntity(Entity* var1, float var2);
	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual Entity* findPlayerToAttack();

private:
	PathEntity* m_pathToEntity;

};

#endif // ENTITYCREATURE_H