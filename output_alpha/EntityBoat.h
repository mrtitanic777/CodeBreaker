#ifndef ENTITYBOAT_H
#define ENTITYBOAT_H

// Auto-generated from EntityBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

struct AxisAlignedBB;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntityBoat : public Entity {
public:
	EntityBoat();
	EntityBoat(World* var1);
	EntityBoat(World* var1, double var2, double var4, double var6);
	virtual ~EntityBoat();

	int m_field_807_a;
	int m_field_806_b;
	int m_field_808_c;

	virtual AxisAlignedBB func_383_b_(Entity* var1);
	virtual AxisAlignedBB func_372_f_();
	virtual bool canBePushed();
	virtual double func_402_h();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void func_9280_g();
	virtual bool canBeCollidedWith();
	virtual void setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void func_366_i_();
	virtual float func_392_h_();
	virtual bool interact(EntityPlayer* var1);

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was private
	int m_field_9394_d;
	double m_field_9393_e;
	double m_field_9392_f;
	double m_field_9391_g;
	double m_field_9390_h;
	double m_field_9389_i;
	double m_field_9388_j;
	double m_field_9387_k;
	double m_field_9386_l;

};

#endif // ENTITYBOAT_H