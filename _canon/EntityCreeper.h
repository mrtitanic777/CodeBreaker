#ifndef ENTITYCREEPER_H
#define ENTITYCREEPER_H

// Auto-generated from EntityCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityMobs.h"

class Entity;
class NBTTagCompound;
class World;

class EntityCreeper : public EntityMobs {
public:
	EntityCreeper(World* var1);
	virtual ~EntityCreeper();

	int timeSinceIgnited;
	int lastActiveTime;
	int fuseTime;
	int creeperState;
	int prevCreeperState;

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void handleHealthUpdate(unsigned char var1);
	virtual void onUpdate();
	virtual void updateEntityActionState();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual void onDeath(Entity* var1);
	virtual void attackEntity(Entity* var1, float var2);
	virtual float getCreeperFlashIntensity(float var1);
	virtual int getDropItemId();

	static void initStatics();
};

#endif // ENTITYCREEPER_H