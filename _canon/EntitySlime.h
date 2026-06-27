#ifndef ENTITYSLIME_H
#define ENTITYSLIME_H

// Auto-generated from EntitySlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityLiving.h"
#include "IMobs.h"

class EntityPlayer;
class NBTTagCompound;
class World;

class EntitySlime : public EntityLiving, public IMobs {
public:
	EntitySlime(World* var1);
	virtual ~EntitySlime();

	float squishFactor;
	float prevSquishFactor;
	int slimeJumpDelay;
	int slimeSize;

	virtual void setSlimeSize(int var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onUpdate();
	virtual void updateEntityActionState();
	virtual void setEntityDead();
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual int getDropItemId();
	virtual bool getCanSpawnHere();
	virtual float getSoundVolume();

	static void initStatics();
};

#endif // ENTITYSLIME_H