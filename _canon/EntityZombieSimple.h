#ifndef ENTITYZOMBIESIMPLE_H
#define ENTITYZOMBIESIMPLE_H

// Auto-generated from EntityZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityMobs.h"

class World;

class EntityZombieSimple : public EntityMobs {
public:
	EntityZombieSimple(World* var1);
	virtual ~EntityZombieSimple();

	virtual float getBlockPathWeight(int var1, int var2, int var3);

	static void initStatics();
};

#endif // ENTITYZOMBIESIMPLE_H