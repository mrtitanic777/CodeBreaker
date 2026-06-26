#ifndef ENTITYSPIDER_H
#define ENTITYSPIDER_H

// Auto-generated from EntitySpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMob.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntitySpider : public EntityMob {
public:
	EntitySpider();
	EntitySpider(World* var1);
	virtual ~EntitySpider();


protected:
	virtual Entity* findPlayerToAttack();
	virtual void attackEntity(Entity* var1, float var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

};

#endif // ENTITYSPIDER_H