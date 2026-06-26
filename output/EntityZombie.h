#ifndef ENTITYZOMBIE_H
#define ENTITYZOMBIE_H

// Auto-generated from EntityZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMob.h"
#include <string>

class World;

class EntityZombie : public EntityMob {
public:
	EntityZombie();
	EntityZombie(World* var1);
	virtual ~EntityZombie();

	virtual void onLivingUpdate();

public: // was protected
	virtual const std::string& getEntityString();

};

#endif // ENTITYZOMBIE_H