#ifndef ENTITY_H
#define ENTITY_H

// Auto-generated from Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "JavaRT.h"

class AxisAlignedBB;
class EntityItem;
class EntityPlayer;
class Material;
class NBTTagCompound;
class NBTTagList;
class Random;
class Vec3D;
class World;

class Entity {
public:
	Entity(World* var1);
	virtual ~Entity();

	int entityId;
	double renderDistanceWeight;
	bool preventEntitySpawning;
	Entity* riddenByEntity;
	Entity* ridingEntity;
	World* worldObj;
	double prevPosX;
	double prevPosY;
	double prevPosZ;
	double posX;
	double posY;
	double posZ;
	double motionX;
	double motionY;
	double motionZ;
	float rotationYaw;
	float rotationPitch;
	float prevRotationYaw;
	float prevRotationPitch;
	AxisAlignedBB* boundingBox;
	bool onGround;
	bool isCollidedHorizontally;
	bool isCollidedVertically;
	bool isCollided;
	bool velocityChanged;
	bool field_9293_aM;
	bool isDead;
	float yOffset;
	float width;
	float height;
	float prevDistanceWalkedModified;
	float distanceWalkedModified;
	bool entityWalks;
	float fallDistance;
	int nextStepDistance;
	double lastTickPosX;
	double lastTickPosY;
	double lastTickPosZ;
	float ySize;
	float stepHeight;
	bool noClip;
	float entityCollisionReduction;
	bool field_9313_bc;
	Random* rand;
	int ticksExisted;
	int fireResistance;
	int fire;
	int maxAir;
	bool inWater;
	int hurtResistantTime;
	int air;
	bool firstUpdate;
	JavaString skinUrl;
	bool isImmuneToFire;
	double minecartType;
	double rotationYawDelta;
	bool addedToChunk;
	int chunkCoordX;
	int chunkCoordY;
	int chunkCoordZ;
	int serverPosX;
	int serverPosY;
	int serverPosZ;
	bool field_9300_bu;
	bool field_9299_bv;
	bool isSneakingRender;
	static int s_nextEntityId;

	virtual bool equals(JObject var1);
	virtual int hashCode();
	virtual void preparePlayerToSpawn();
	virtual void setEntityDead();
	virtual void setSize(float var1, float var2);
	virtual void setRotation(float var1, float var2);
	virtual void setPosition(double var1, double var3, double var5);
	virtual void setAngles(float var1, float var2);
	virtual void onUpdate();
	virtual void onEntityUpdate();
	virtual void setOnFireFromLava();
	virtual void outOfWorld();
	virtual bool isOffsetPositionInLiquid(double var1, double var3, double var5);
	virtual void moveEntity(double var1, double var3, double var5);
	virtual void updateFallState(double var1, bool var3);
	virtual bool isSneaking();
	virtual AxisAlignedBB* getCollisionBox();
	virtual void dealFireDamage(int var1);
	virtual void fall(float var1);
	virtual bool handleWaterMovement();
	virtual bool isInsideOfMaterial(Material* var1);
	virtual float getEyeHeight();
	virtual bool handleLavaMovement();
	virtual void moveFlying(float var1, float var2, float var3);
	virtual float getEntityBrightness(float var1);
	virtual void setWorld(World* var1);
	virtual void setPositionAndRotation(double var1, double var3, double var5, float var7, float var8);
	virtual void setLocationAndAngles(double var1, double var3, double var5, float var7, float var8);
	virtual float getDistanceToEntity(Entity* var1);
	virtual double getDistanceSq(double var1, double var3, double var5);
	virtual double getDistance(double var1, double var3, double var5);
	virtual double getDistanceSqToEntity(Entity* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual void applyEntityCollision(Entity* var1);
	virtual void addVelocity(double var1, double var3, double var5);
	virtual void setBeenAttacked();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual bool canBeCollidedWith();
	virtual bool canBePushed();
	virtual void addToPlayerScore(Entity* var1, int var2);
	virtual bool isInRangeToRender3d(Vec3D* var1);
	virtual bool isInRangeToRenderDist(double var1);
	virtual JavaString getEntityTexture();
	virtual bool addEntityID(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual JavaString getEntityString();
	virtual void readEntityFromNBT(NBTTagCompound* var1) = 0;
	virtual void writeEntityToNBT(NBTTagCompound* var1) = 0;
	virtual NBTTagList* dropChunk(JArray<double > var1);
	virtual NBTTagList* newFloatNBTList(JArray<float > var1);
	virtual float getYOffsetForRender();
	virtual EntityItem* dropItem(int var1, int var2);
	virtual EntityItem* dropItemWithOffset(int var1, int var2, float var3);
	virtual bool isEntityAlive();
	virtual bool isEntityInsideOpaqueBlock();
	virtual bool interact(EntityPlayer* var1);
	virtual AxisAlignedBB* getBoundingBox(Entity* var1);
	virtual void updateRidden();
	virtual void updateRiderPosition();
	virtual double getYOffset();
	virtual double getMountedYOffset();
	virtual void mountEntity(Entity* var1);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual float getShadowSize();
	virtual Vec3D* getLookVec();
	virtual void setInPortal();
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void handleHealthUpdate(unsigned char var1);
	virtual void performHurtAnimation();

	static void initStatics();
};

#endif // ENTITY_H