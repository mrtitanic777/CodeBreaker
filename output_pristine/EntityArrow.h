#ifndef ENTITYARROW_H
#define ENTITYARROW_H

// Auto-generated from EntityArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Entity.h"
#include <string>

class EntityLiving;
class EntityPlayer;
class NBTTagCompound;
class World;

class EntityArrow : public Entity {
public:
	EntityArrow();
	EntityArrow(World* var1, EntityLiving* var2);
	virtual ~EntityArrow();

	int m_arrowShake;

	virtual void setArrowHeading(float var1, float var2, float var3, float var4, float var5);
	virtual void onEntityUpdate();
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual float getShadowSize();

protected:
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

private:
	int m_xTile;
	int m_yTile;
	int m_zTile;
	int m_inTile;
	bool m_inGround;
	EntityLiving* m_owner;
	int m_ticksInGround;
	int m_ticksInAir;

};

#endif // ENTITYARROW_H