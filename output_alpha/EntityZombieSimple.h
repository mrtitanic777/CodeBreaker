#ifndef ENTITYZOMBIESIMPLE_H
#define ENTITYZOMBIESIMPLE_H

// Auto-generated from EntityZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"

class World;

class EntityZombieSimple : public EntityMobs {
public:
	EntityZombieSimple();
	EntityZombieSimple(World* var1);
	virtual ~EntityZombieSimple();


public: // was protected
	virtual float func_439_a(int var1, int var2, int var3);

};

#endif // ENTITYZOMBIESIMPLE_H