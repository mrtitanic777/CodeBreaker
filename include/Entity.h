#ifndef ENTITY_H
#define ENTITY_H

// Auto-generated from Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "AxisAlignedBB.h"
#include <string>

class EntityItem;
class EntityPlayer;
class NBTTagCompound;
class NBTTagList;
class Random;
class World;

class Entity {
public:
	Entity();
	Entity(World* var1);
	virtual ~Entity();

	bool preventEntitySpawning;
	float prevPosX;
	float prevPosY;
	float prevPosZ;
	float posX;
	float posY;
	float posZ;
	float motionX;
	float motionY;
	float motionZ;
	float rotationYaw;
	float rotationPitch;
	float prevRotationYaw;
	float prevRotationPitch;
	AxisAlignedBB boundingBox;
	bool onGround;
	bool isCollidedHorizontally;
	bool isDead;
	float yOffset;
	float width;
	float height;
	float prevDistanceWalkedModified;
	float distanceWalkedModified;
	float lastTickPosX;
	float lastTickPosY;
	float lastTickPosZ;
	float stepHeight;
	bool noClip;
	int ticksExisted;
	int fireResistance;
	int fire;
	int heartsLife;
	int air;
	std::string skinUrl;

	virtual void setEntityDead();
	virtual void onEntityUpdate();
	virtual bool isOffsetPositionInLiquid(float var1, float var2, float var3);
	virtual void moveEntity(float var1, float var2, float var3);
	virtual bool handleWaterMovement();
	virtual bool isInsideOfMaterial();
	virtual bool handleLavaMovement();
	virtual void moveFlying(float var1, float var2, float var3);
	virtual float getEntityBrightness(float var1);
	virtual void setWorld(World* var1);
	virtual void setPositionAndRotation(float var1, float var2, float var3, float var4, float var5);
	virtual float getDistanceSqToEntity(Entity* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual void applyEntityCollision(Entity* var1);
	virtual bool attackEntityFrom(Entity* var1, int var2);
	virtual bool canBeCollidedWith();
	virtual bool canBePushed();
	virtual const std::string& getTexture();
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual float getShadowSize();
	virtual EntityItem* dropItemWithOffset(int var1, int var2);
	virtual EntityItem* entityDropItem(int var1, int var2, float var3);
	virtual bool isEntityAlive();

protected:
	World* worldObj;
	bool canTriggerWalking;
	Random* rand;
	int maxAir;

	virtual void preparePlayerToSpawn();
	virtual void setSize(float var1, float var2);
	virtual void setPosition(float var1, float var2, float var3);
	virtual void dealFireDamage(int var1);
	virtual void fall(float var1);
	virtual float getEyeHeight();
	virtual const std::string& getEntityString() = 0;
	virtual void readEntityFromNBT(NBTTagCompound* var1) = 0;
	virtual void writeEntityToNBT(NBTTagCompound* var1) = 0;

private:
	bool m_surfaceCollision;
	float m_fallDistance;
	int m_nextStepDistance;
	float m_ySize;
	float m_entityCollisionReduction;
	bool m_inWater;
	bool m_isFirstUpdate;

	void addVelocity(float var1, float var2, float var3);
	NBTTagList* newDoubleNBTList(float* var0, int var0_count);

};

#endif // ENTITY_H