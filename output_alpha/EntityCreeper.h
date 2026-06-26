#ifndef ENTITYCREEPER_H
#define ENTITYCREEPER_H

// Auto-generated from EntityCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntityCreeper : public EntityMobs {
public:
	EntityCreeper();
	EntityCreeper(World* var1);
	virtual ~EntityCreeper();

	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void func_9282_a(unsigned char var1);
	virtual void onUpdate();
	virtual void onDeath(Entity* var1);
	virtual float func_440_b(float var1);

public: // was protected
	virtual void func_418_b_();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual void attackEntity(Entity* var1, float var2);
	virtual int getDropItemId();

public: // was private
	int m_timeSinceIgnited;
	int m_lastActiveTime;
	int m_fuseTime;
	int m_creeperState;
	int m_field_12241_e;

};

#endif // ENTITYCREEPER_H