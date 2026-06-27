#ifndef ENTITYBOAT_H
#define ENTITYBOAT_H

// Auto-generated from EntityBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class AxisAlignedBB;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntityBoat : public Entity {
public:
	EntityBoat(World* var1);
	EntityBoat(World* var1, double var2, double var4, double var6);
	virtual ~EntityBoat();

	int damageTaken;
	int timeSinceHit;
	int forwardDirection;
	int boatPosRotationIncrements;
	double boatX;
	double boatY;
	double boatZ;
	double boatYaw;
	double boatPitch;
	double velocityX;
	double velocityY;
	double velocityZ;

	virtual AxisAlignedBB* getBoundingBox(Entity* var1);
	virtual AxisAlignedBB* getCollisionBox();
	virtual bool canBePushed();
	virtual double getMountedYOffset();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void performHurtAnimation();
	virtual bool canBeCollidedWith();
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void updateRiderPosition();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual float getYOffsetForRender();
	virtual bool interact(EntityPlayer* var1);

	static void initStatics();
};

#endif // ENTITYBOAT_H