#ifndef ENTITYARROW_H
#define ENTITYARROW_H

// Auto-generated from EntityArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"

class EntityLiving;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntityArrow : public Entity {
public:
	EntityArrow();
	EntityArrow(World* var1);
	EntityArrow(World* var1, double var2, double var4, double var6);
	EntityArrow(World* var1, EntityLiving* var2);
	virtual ~EntityArrow();

	int m_arrowShake;
	EntityLiving* m_field_682_g;

	virtual void func_408_a(double var1, double var3, double var5, float var7, float var8);
	virtual void setVelocity(double var1, double var3, double var5);
	virtual void onUpdate();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual float func_392_h_();

public: // was private
	int m_xTile;
	int m_yTile;
	int m_zTile;
	int m_inTile;
	bool m_inGround;
	int m_field_681_h;
	int m_field_680_i;

};

#endif // ENTITYARROW_H