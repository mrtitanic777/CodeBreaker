#ifndef ENTITYSHEEP_H
#define ENTITYSHEEP_H

// Auto-generated from EntitySheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimals.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntitySheep : public EntityAnimals {
public:
	EntitySheep();
	EntitySheep(World* var1);
	virtual ~EntitySheep();

	bool m_sheared;

	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was protected
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();

};

#endif // ENTITYSHEEP_H