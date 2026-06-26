#ifndef ENTITY_H
#define ENTITY_H

// Auto-generated from Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "AxisAlignedBB.h"
#include <string>

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
	Entity();
	Entity(World* var1);
	virtual ~Entity();

	int m_field_620_ab;
	double m_field_619_ac;
	bool m_field_618_ad;
	Entity* m_riddenByEntity;
	Entity* m_ridingEntity;
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
	AxisAlignedBB boundingBox;
	bool onGround;
	bool m_field_9297_aI;
	bool m_field_9296_aJ;
	bool m_field_9295_aK;
	bool m_field_9294_aL;
	bool m_field_9293_aM;
	bool isDead;
	float yOffset;
	float width;
	float height;
	float m_field_9291_aR;
	float m_field_9290_aS;
	double lastTickPosX;
	double lastTickPosY;
	double lastTickPosZ;
	float m_field_9287_aY;
	float m_field_9286_aZ;
	bool m_field_9314_ba;
	float m_field_632_aO;
	bool m_field_9313_bc;
	int m_field_9311_be;
	int m_field_9310_bf;
	int fire;
	int m_field_9306_bj;
	int air;
	std::string skinUrl;
	bool m_field_621_aZ;
	int m_field_657_ba;
	int m_field_656_bb;
	int m_field_654_bc;
	int m_field_9303_br;
	int m_field_9302_bs;
	int m_field_9301_bt;
	bool m_field_9300_bu;
	bool m_field_9299_bv;
	bool m_field_12240_bw;

	virtual bool equals(void* /* PORT: was Object */ var1);
	virtual int hashCode();
	virtual void setEntityDead();
	virtual void setPosition(double var1, double var3, double var5);
	virtual void func_346_d(float var1, float var2);
	virtual void onUpdate();
	virtual void func_391_y();
	virtual bool func_403_b(double var1, double var3, double var5);
	virtual void moveEntity(double var1, double var3, double var5);
	virtual bool func_381_o();
	virtual AxisAlignedBB func_372_f_();
	virtual bool handleWaterMovement();
	virtual bool isInsideOfMaterial(Material* var1);
	virtual float func_373_s();
	virtual bool handleLavaMovement();
	virtual void func_351_a(float var1, float var2, float var3);
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
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual bool canBeCollidedWith();
	virtual bool canBePushed();
	virtual void addToPlayerScore(Entity* var1, int var2);
	virtual bool func_390_a(Vec3D* var1);
	virtual bool func_384_a(double var1);
	virtual const std::string& getEntityTexture();
	virtual bool func_358_c(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual float func_392_h_();
	virtual EntityItem* dropItem(int var1, int var2);
	virtual EntityItem* dropItemWithOffset(int var1, int var2, float var3);
	virtual bool isEntityAlive();
	virtual bool func_345_I();
	virtual bool interact(EntityPlayer* var1);
	virtual AxisAlignedBB func_383_b_(Entity* var1);
	virtual void func_350_p();
	virtual void func_366_i_();
	virtual double func_388_v();
	virtual double func_402_h();
	virtual void mountEntity(Entity* var1);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual float func_4035_j_();
	virtual Vec3D* func_4037_H();
	virtual void func_4039_q();
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void func_9282_a(unsigned char var1);
	virtual void func_9280_g();

public: // was protected
	bool m_entityWalks;
	float m_fallDistance;
	Random* rand;
	int m_field_9308_bh;
	bool m_field_9307_bi;
	bool m_isImmuneToFire;

	virtual void preparePlayerToSpawn();
	virtual void setSize(float var1, float var2);
	virtual void setRotation(float var1, float var2);
	virtual void func_4038_J();
	virtual void func_4034_G();
	virtual void func_9279_a(double var1, bool var3);
	virtual void func_355_a(int var1);
	virtual void fall(float var1);
	virtual void func_9281_M();
	virtual const std::string& getEntityString();
	virtual void readEntityFromNBT(NBTTagCompound* var1) = 0;
	virtual void writeEntityToNBT(NBTTagCompound* var1) = 0;
	virtual NBTTagList* func_375_a(double* var1, int var1_count);
	virtual NBTTagList* func_377_a(float* var1, int var1_count);

public: // was private
	static int s_field_864_a;
	int m_field_863_b;
	bool m_field_862_c;
	double m_minecartType;
	double m_field_667_e;

};

#endif // ENTITY_H