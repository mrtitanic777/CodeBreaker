#ifndef EXPLOSION_H
#define EXPLOSION_H

// Auto-generated from Explosion.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <set>

class Entity;
class Random;
class World;

class Explosion {
public:
	Explosion(World* var1, Entity* var2, double var3, double var5, double var7, float var9);
	virtual ~Explosion();

	bool isFlaming;
	Random* explosionRNG;
	World* worldObj;
	double explosionX;
	double explosionY;
	double explosionZ;
	Entity* exploder;
	float explosionSize;
	std::set<void*> destroyedBlockPositions;

	virtual void doExplosionA();
	virtual void doExplosionB();

	static void initStatics();
};

#endif // EXPLOSION_H