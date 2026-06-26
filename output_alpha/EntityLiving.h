#ifndef ENTITYLIVING_H
#define ENTITYLIVING_H

// Auto-generated from EntityLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include <string>

class ItemStack;
class MovingObjectPosition;
class NBTTagCompound;
class Vec3D;
class World;

class EntityLiving : public Entity {
public:
	EntityLiving();
	EntityLiving(World* var1);
	virtual ~EntityLiving();

	int m_field_9366_o;
	float m_field_9365_p;
	float m_field_9364_q;
	float m_field_9363_r;
	float m_field_735_n;
	float m_field_734_o;
	bool m_field_9343_G;
	float prevSwingProgress;
	float swingProgress;
	int health;
	int m_field_9335_K;
	int hurtTime;
	int m_field_9332_M;
	float m_field_9331_N;
	int deathTime;
	int attackTime;
	float m_field_9329_Q;
	float m_field_9328_R;
	int m_field_9326_T;
	float m_field_9325_U;
	float m_field_705_Q;
	float m_field_704_R;
	float m_field_703_S;

	virtual bool canEntityBeSeen(Entity* var1);
	virtual const std::string& getEntityTexture();
	virtual bool canBeCollidedWith();
	virtual bool canBePushed();
	virtual float func_373_s();
	virtual int func_421_b();
	virtual void func_391_y();
	virtual void spawnExplosionParticle();
	virtual void func_350_p();
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void onUpdate();
	virtual void heal(int var1);
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void func_9280_g();
	virtual void func_434_a(Entity* var1, int var2, double var3, double var5);
	virtual void onDeath(Entity* var1);
	virtual void func_435_b(float var1, float var2);
	virtual bool isOnLadder();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool isEntityAlive();
	virtual void onLivingUpdate();
	virtual void faceEntity(Entity* var1, float var2);
	virtual void func_6392_F();
	virtual bool getCanSpawnHere();
	virtual float getSwingProgress(float var1);
	virtual Vec3D* getPosition(float var1);
	virtual Vec3D* func_4037_H();
	virtual Vec3D* getLook(float var1);
	virtual MovingObjectPosition* rayTrace(double var1, float var3);
	virtual int func_6391_i();
	virtual ItemStack* getHeldItem();
	virtual void func_9282_a(unsigned char var1);

public: // was protected
	float m_field_9362_u;
	float m_field_9361_v;
	float m_field_9360_w;
	float m_field_9359_x;
	bool m_field_9358_y;
	std::string texture;
	bool m_field_9355_A;
	float m_field_9353_B;
	std::string m_field_9351_C;
	float m_field_9349_D;
	int m_field_9347_E;
	float m_field_9345_F;
	bool m_field_9327_S;
	int m_field_9324_Y;
	double m_field_9323_Z;
	double m_field_9356_aa;
	double m_field_9354_ab;
	double m_field_9352_ac;
	double m_field_9350_ad;
	int m_field_9346_af;
	int m_field_9344_ag;
	float m_field_9342_ah;
	float m_field_9340_ai;
	float m_field_9338_aj;
	bool isJumping;
	float m_field_9334_al;
	float m_field_9333_am;

	virtual void setSize(float var1, float var2);
	virtual void damageEntity(int var1);
	virtual float getSoundVolume();
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();
	virtual void fall(float var1);
	virtual void func_424_C();
	virtual void func_418_b_();
	virtual void func_4034_G();

public: // was private
	int m_a;
	float m_field_9348_ae;
	Entity* m_b;
	int m_c;

	float updateRotation(float var1, float var2, float var3);

};

#endif // ENTITYLIVING_H