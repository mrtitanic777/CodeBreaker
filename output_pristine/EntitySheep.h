#ifndef ENTITYSHEEP_H
#define ENTITYSHEEP_H

// Auto-generated from EntitySheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimal.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntitySheep : public EntityAnimal {
public:
	EntitySheep();
	EntitySheep(World* var1);
	virtual ~EntitySheep();

	bool m_sheared;

	virtual bool attackEntityFrom(Entity* var1, int var2);

protected:
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();

};

#endif // ENTITYSHEEP_H