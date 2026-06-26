#ifndef ENTITYGIANTZOMBIE_H
#define ENTITYGIANTZOMBIE_H

// Auto-generated from EntityGiantZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMob.h"
#include <string>

class World;

class EntityGiantZombie : public EntityMob {
public:
	EntityGiantZombie();
	EntityGiantZombie(World* var1);
	virtual ~EntityGiantZombie();


protected:
	virtual float getBlockPathWeight(int var1, int var2, int var3);
	virtual const std::string& getEntityString();

};

#endif // ENTITYGIANTZOMBIE_H