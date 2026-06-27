#ifndef ENTITYLIVING_H
#define ENTITYLIVING_H

// Auto-generated from EntityLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Entity.h"

class ItemStack;
class MovingObjectPosition;
class NBTTagCompound;
class Vec3D;
class World;

class EntityLiving : public Entity {
public:
	EntityLiving(World* var1);
	virtual ~EntityLiving();

	int maxHurtResistantTime;
	float randomEntitySeed;
	float field_9364_q;
	float field_9363_r;
	float renderYawOffset;
	float prevRenderYawOffset;
	float prevOnGroundSpeedFactor;
	float onGroundSpeedFactor;
	float legYaw;
	float prevLegYaw;
	bool field_9358_y;
	JavaString texture;
	bool field_9355_A;
	float renderYawOffsetBase;
	JavaString entityType;
	float field_9349_D;
	int scoreValue;
	float field_9345_F;
	bool noAi;
	float prevSwingProgress;
	float swingProgress;
	int health;
	int prevHealth;
	int a;
	int hurtTime;
	int maxHurtTime;
	float attackedAtYaw;
	int deathTime;
	int attackTime;
	float field_9329_Q;
	float field_9328_R;
	bool dead;
	int field_9326_T;
	float field_9325_U;
	float prevLimbSwingAmount;
	float limbSwingAmount;
	float limbSwing;
	int newPosRotationIncrements;
	double newPosX;
	double newPosY;
	double newPosZ;
	double newRotationYaw;
	double newRotationPitch;
	float field_9348_ae;
	int lastDamage;
	int entityAge;
	float moveStrafing;
	float moveForward;
	float randomYawVelocity;
	bool isJumping;
	float defaultPitch;
	float moveSpeed;
	Entity* b;
	int c;

	virtual bool canEntityBeSeen(Entity* var1);
	virtual JavaString getEntityTexture();
	virtual bool canBeCollidedWith();
	virtual bool canBePushed();
	virtual float getEyeHeight();
	virtual int getTalkInterval();
	virtual void onEntityUpdate();
	virtual void spawnExplosionParticle();
	virtual void updateRidden();
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void onUpdate();
	virtual void setSize(float var1, float var2);
	virtual void heal(int var1);
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void performHurtAnimation();
	virtual void damageEntity(int var1);
	virtual float getSoundVolume();
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual void knockBack(Entity* var1, int var2, double var3, double var5);
	virtual void onDeath(Entity* var1);
	virtual int getDropItemId();
	virtual void fall(float var1);
	virtual void moveEntityWithHeading(float var1, float var2);
	virtual bool isOnLadder();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool isEntityAlive();
	virtual void onLivingUpdate();
	virtual void jump();
	virtual void updateEntityActionState();
	virtual void faceEntity(Entity* var1, float var2);
	virtual float updateRotation(float var1, float var2, float var3);
	virtual void onEntityDeath();
	virtual bool getCanSpawnHere();
	virtual void outOfWorld();
	virtual float getSwingProgress(float var1);
	virtual Vec3D* getPosition(float var1);
	virtual Vec3D* getLookVec();
	virtual Vec3D* getLook(float var1);
	virtual MovingObjectPosition* rayTrace(double var1, float var3);
	virtual int getMaxSpawnedInChunk();
	virtual ItemStack* getHeldItem();
	virtual void handleHealthUpdate(unsigned char var1);

	static void initStatics();
};

#endif // ENTITYLIVING_H