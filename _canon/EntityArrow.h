#ifndef ENTITYARROW_H
#define ENTITYARROW_H

// Auto-generated from EntityArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class EntityLiving;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntityArrow : public Entity {
public:
	EntityArrow(World* var1);
	EntityArrow(World* var1, double var2, double var4, double var6);
	EntityArrow(World* var1, EntityLiving* var2);
	virtual ~EntityArrow();

	int xTile;
	int yTile;
	int zTile;
	int inTile;
	bool inGround;
	int arrowShake;
	EntityLiving* shootingEntity;
	int ticksInGround;
	int ticksInAir;

	virtual void setArrowHeading(double var1, double var3, double var5, float var7, float var8);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual float getYOffsetForRender();

	static void initStatics();
};

#endif // ENTITYARROW_H