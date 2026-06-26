#ifndef ENTITYSPIDER_H
#define ENTITYSPIDER_H

// Auto-generated from EntitySpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"
#include <string>

class Entity;
class NBTTagCompound;
class World;

class EntitySpider : public EntityMobs {
public:
	EntitySpider();
	EntitySpider(World* var1);
	virtual ~EntitySpider();

	virtual double func_402_h();
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);

public: // was protected
	virtual Entity* findPlayerToAttack();
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual void attackEntity(Entity* var1, float var2);
	virtual int getDropItemId();

};

#endif // ENTITYSPIDER_H