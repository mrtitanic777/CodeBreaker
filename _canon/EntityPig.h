#ifndef ENTITYPIG_H
#define ENTITYPIG_H

// Auto-generated from EntityPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityAnimals.h"

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityPig : public EntityAnimals {
public:
	EntityPig(World* var1);
	virtual ~EntityPig();

	bool rideable;

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual bool interact(EntityPlayer* var1);
	virtual int getDropItemId();

	static void initStatics();
};

#endif // ENTITYPIG_H