#ifndef ENTITYSLIME_H
#define ENTITYSLIME_H

// Auto-generated from EntitySlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"
#include "IMobs.h"
#include <string>

class EntityPlayer;
class NBTTagCompound;
class World;

class EntitySlime : public EntityLiving, public IMobs {
public:
	EntitySlime();
	EntitySlime(World* var1);
	virtual ~EntitySlime();

	float m_field_768_a;
	float m_field_767_b;
	int m_field_770_c;

	virtual void func_441_c(int var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void onUpdate();
	virtual void setEntityDead();
	virtual void onCollideWithPlayer(EntityPlayer* var1);
	virtual bool getCanSpawnHere();

public: // was protected
	virtual void func_418_b_();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();
	virtual float getSoundVolume();

public: // was private
	int m_field_769_d;

};

#endif // ENTITYSLIME_H