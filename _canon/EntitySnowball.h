#ifndef ENTITYSNOWBALL_H
#define ENTITYSNOWBALL_H

// Auto-generated from EntitySnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class EntityLiving;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntitySnowball : public Entity {
public:
	EntitySnowball(World* var1);
	EntitySnowball(World* var1, EntityLiving* var2);
	EntitySnowball(World* var1, double var2, double var4, double var6);
	virtual ~EntitySnowball();

	int xTile;
	int yTile;
	int zTile;
	int inTile;
	bool inGround;
	int arrowShake;
	EntityLiving* thrower;
	int ticksInGround;
	int ticksInAir;

	virtual bool isInRangeToRenderDist(double var1);
	virtual void setThrowableHeading(double var1, double var3, double var5, float var7, float var8);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual float getYOffsetForRender();

	static void initStatics();
};

#endif // ENTITYSNOWBALL_H