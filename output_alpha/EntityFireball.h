#ifndef ENTITYFIREBALL_H
#define ENTITYFIREBALL_H

// Auto-generated from EntityFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class EntityLiving;
class NBTTagCompound;
class World;

class EntityFireball : public Entity {
public:
	EntityFireball();
	EntityFireball(World* var1);
	EntityFireball(World* var1, EntityLiving* var2, double var3, double var5, double var7);
	virtual ~EntityFireball();

	int m_field_9406_a;
	double m_field_9405_b;
	double m_field_9404_c;
	double m_field_9403_d;

	virtual bool func_384_a(double var1);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual bool canBeCollidedWith();
	virtual float func_4035_j_();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual float func_392_h_();

public: // was private
	int m_field_9402_e;
	int m_field_9401_f;
	int m_field_9400_g;
	int m_field_9399_h;
	bool m_field_9398_i;
	EntityLiving* m_field_9397_j;
	int m_field_9396_k;
	int m_field_9395_l;

};

#endif // ENTITYFIREBALL_H