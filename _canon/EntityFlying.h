#ifndef ENTITYFLYING_H
#define ENTITYFLYING_H

// Auto-generated from EntityFlying.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityLiving.h"

class World;

class EntityFlying : public EntityLiving {
public:
	EntityFlying(World* var1);
	virtual ~EntityFlying();

	virtual void fall(float var1);
	virtual void moveEntityWithHeading(float var1, float var2);
	virtual bool isOnLadder();

	static void initStatics();
};

#endif // ENTITYFLYING_H