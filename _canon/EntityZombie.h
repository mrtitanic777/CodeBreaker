#ifndef ENTITYZOMBIE_H
#define ENTITYZOMBIE_H

// Auto-generated from EntityZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityMobs.h"

class World;

class EntityZombie : public EntityMobs {
public:
	EntityZombie(World* var1);
	virtual ~EntityZombie();

	virtual void onLivingUpdate();
	virtual JavaString getLivingSound();
	virtual JavaString getHurtSound();
	virtual JavaString getDeathSound();
	virtual int getDropItemId();

	static void initStatics();
};

#endif // ENTITYZOMBIE_H