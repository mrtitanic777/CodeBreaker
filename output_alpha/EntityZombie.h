#ifndef ENTITYZOMBIE_H
#define ENTITYZOMBIE_H

// Auto-generated from EntityZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"
#include <string>

class World;

class EntityZombie : public EntityMobs {
public:
	EntityZombie();
	EntityZombie(World* var1);
	virtual ~EntityZombie();

	virtual void onLivingUpdate();

public: // was protected
	virtual const std::string& getLivingSound();
	virtual const std::string& getHurtSound();
	virtual const std::string& getDeathSound();
	virtual int getDropItemId();

};

#endif // ENTITYZOMBIE_H