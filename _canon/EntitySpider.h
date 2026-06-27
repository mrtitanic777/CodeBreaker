#ifndef ENTITYSPIDER_H
#define ENTITYSPIDER_H

// Auto-generated from EntitySpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityMobs.h"

class Entity;
class NBTTagCompound;
class World;

class EntitySpider : public EntityMobs {
public:
	EntitySpider(World* var1);
	virtual ~EntitySpider();

	virtual double getMountedYOffset();
	virtual Entity* findPlayerToAttack();
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual void attackEntity(Entity* var1, float var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual int getDropItemId();

	static void initStatics();
};

#endif // ENTITYSPIDER_H