#ifndef ENTITYCOW_H
#define ENTITYCOW_H

// Auto-generated from EntityCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityAnimals.h"

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityCow : public EntityAnimals {
public:
	EntityCow(World* var1);
	virtual ~EntityCow();

	bool unusedBoolean;

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual float getSoundVolume();
	virtual int getDropItemId();
	virtual bool interact(EntityPlayer* var1);

	static void initStatics();
};

#endif // ENTITYCOW_H