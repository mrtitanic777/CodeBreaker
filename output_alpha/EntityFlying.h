#ifndef ENTITYFLYING_H
#define ENTITYFLYING_H

// Auto-generated from EntityFlying.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"

class World;

class EntityFlying : public EntityLiving {
public:
	EntityFlying();
	EntityFlying(World* var1);
	virtual ~EntityFlying();

	virtual void func_435_b(float var1, float var2);
	virtual bool isOnLadder();

public: // was protected
	virtual void fall(float var1);

};

#endif // ENTITYFLYING_H