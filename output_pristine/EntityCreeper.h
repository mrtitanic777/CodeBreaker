#ifndef ENTITYCREEPER_H
#define ENTITYCREEPER_H

// Auto-generated from EntityCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMob.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntityCreeper : public EntityMob {
public:
	EntityCreeper();
	EntityCreeper(World* var1);
	virtual ~EntityCreeper();

	virtual float c(float var1);

protected:
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();
	virtual void updatePlayerActionState();
	virtual void attackEntity(Entity* var1, float var2);

private:
	int m_timeSinceIgnited;
	int m_lastActiveTime;
	int m_fuseTime;
	int m_creeperState;

};

#endif // ENTITYCREEPER_H