#ifndef ENTITYFIREBALL_H
#define ENTITYFIREBALL_H

// Auto-generated from EntityFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class EntityLiving;
class NBTTagCompound;
class World;

class EntityFireball : public Entity {
public:
	EntityFireball(World* var1);
	EntityFireball(World* var1, EntityLiving* var2, double var3, double var5, double var7);
	virtual ~EntityFireball();

	int xTile;
	int yTile;
	int zTile;
	int inTile;
	bool inGround;
	int arrowShake;
	EntityLiving* shootingEntity;
	int ticksAlive;
	int ticksInAir;
	double accelerationX;
	double accelerationY;
	double accelerationZ;

	virtual bool isInRangeToRenderDist(double var1);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool canBeCollidedWith();
	virtual float getShadowSize();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual float getYOffsetForRender();

	static void initStatics();
};

#endif // ENTITYFIREBALL_H