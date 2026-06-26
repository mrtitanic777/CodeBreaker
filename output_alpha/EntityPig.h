#ifndef ENTITYPIG_H
#define ENTITYPIG_H

// Auto-generated from EntityPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimals.h"
#include <string>

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityPig : public EntityAnimals {
public:
	EntityPig();
	EntityPig(World* var1);
	virtual ~EntityPig();

	bool m_rideable;

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool interact(EntityPlayer* var1);

public: // was protected
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();

};

#endif // ENTITYPIG_H