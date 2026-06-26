#ifndef ENTITYPIG_H
#define ENTITYPIG_H

// Auto-generated from EntityPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimal.h"
#include <string>

class NBTTagCompound;
class World;

class EntityPig : public EntityAnimal {
public:
	EntityPig();
	EntityPig(World* var1);
	virtual ~EntityPig();


public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();

};

#endif // ENTITYPIG_H