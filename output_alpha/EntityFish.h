#ifndef ENTITYFISH_H
#define ENTITYFISH_H

// Auto-generated from EntityFish.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class EntityPlayer;
class NBTTagCompound;
class World;

class EntityFish : public Entity {
public:
	EntityFish();
	EntityFish(World* var1);
	EntityFish(World* var1, double var2, double var4, double var6);
	EntityFish(World* var1, EntityPlayer* var2);
	virtual ~EntityFish();

	int m_field_4098_a;
	EntityPlayer* m_field_4097_b;
	Entity* m_field_4096_c;

	virtual bool func_384_a(double var1);
	virtual void func_4042_a(double var1, double var3, double var5, float var7, float var8);
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual float func_392_h_();
	virtual int func_4043_i();

public: // was private
	int m_field_4095_d;
	int m_field_4094_e;
	int m_field_4093_f;
	int m_field_4092_g;
	bool m_field_4091_h;
	int m_field_4090_i;
	int m_field_4089_j;
	int m_field_4088_k;
	int m_field_6388_l;
	double m_field_6387_m;
	double m_field_6386_n;
	double m_field_6385_o;
	double m_field_6384_p;
	double m_field_6383_q;
	double m_velocityX;
	double m_velocityY;
	double m_velocityZ;

};

#endif // ENTITYFISH_H