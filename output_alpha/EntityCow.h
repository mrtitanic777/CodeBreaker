#ifndef ENTITYCOW_H
#define ENTITYCOW_H

// Auto-generated from EntityCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimals.h"
#include <string>

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityCow : public EntityAnimals {
public:
	EntityCow();
	EntityCow(World* var1);
	virtual ~EntityCow();

	bool m_unusedBoolean;

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool interact(EntityPlayer* var1);

public: // was protected
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual float getSoundVolume();
	virtual int getDropItemId();

};

#endif // ENTITYCOW_H