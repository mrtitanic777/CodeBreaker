#ifndef ENTITYSKELETON_H
#define ENTITYSKELETON_H

// Auto-generated from EntitySkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMob.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntitySkeleton : public EntityMob {
public:
	EntitySkeleton();
	EntitySkeleton(World* var1);
	virtual ~EntitySkeleton();

	virtual void onLivingUpdate();

protected:
	virtual void attackEntity(Entity* var1, float var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

};

#endif // ENTITYSKELETON_H