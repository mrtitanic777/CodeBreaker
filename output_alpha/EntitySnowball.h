#ifndef ENTITYSNOWBALL_H
#define ENTITYSNOWBALL_H

// Auto-generated from EntitySnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class EntityLiving;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntitySnowball : public Entity {
public:
	EntitySnowball();
	EntitySnowball(World* var1);
	EntitySnowball(World* var1, EntityLiving* var2);
	EntitySnowball(World* var1, double var2, double var4, double var6);
	virtual ~EntitySnowball();

	int m_field_817_a;

	virtual bool func_384_a(double var1);
	virtual void func_467_a(double var1, double var3, double var5, float var7, float var8);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual float func_392_h_();

public: // was private
	int m_field_816_b;
	int m_field_815_c;
	int m_field_814_d;
	int m_field_813_e;
	bool m_field_812_f;
	EntityLiving* m_field_811_g;
	int m_field_810_h;
	int m_field_809_i;

};

#endif // ENTITYSNOWBALL_H