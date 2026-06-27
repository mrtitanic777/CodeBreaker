#ifndef ENTITYFISH_H
#define ENTITYFISH_H

// Auto-generated from EntityFish.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityFish : public Entity {
public:
	EntityFish(World* var1);
	EntityFish(World* var1, double var2, double var4, double var6);
	EntityFish(World* var1, EntityPlayer* var2);
	virtual ~EntityFish();

	int xTile;
	int yTile;
	int zTile;
	int inTile;
	bool inGround;
	int shake;
	EntityPlayer* angler;
	int ticksInGround;
	int ticksInAir;
	int ticksCatchable;
	Entity* caughtEntity;
	int fishPosRotationIncrements;
	double fishX;
	double fishY;
	double fishZ;
	double fishYaw;
	double fishPitch;
	double velocityX;
	double velocityY;
	double velocityZ;

	virtual bool isInRangeToRenderDist(double var1);
	virtual void setThrowableHeading(double var1, double var3, double var5, float var7, float var8);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual float getYOffsetForRender();
	virtual int catchFish();

	static void initStatics();
};

#endif // ENTITYFISH_H