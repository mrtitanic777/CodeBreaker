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

	virtual bool getCanSpawnHere();

public: // was protected
	Entity* m_playerToAttack;
	bool m_hasAttacked;

	virtual void func_418_b_();
	virtual void attackEntity(Entity* var1, float var2);
	virtual float func_439_a(int var1, int var2, int var3);
	virtual Entity* findPlayerToAttack();

public: // was private
	PathEntity* m_pathToEntity;

};

#endif // ENTITYCREATURE_H