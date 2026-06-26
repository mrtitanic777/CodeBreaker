#ifndef WORLDGENFIRE_H
#define WORLDGENFIRE_H

// Auto-generated from WorldGenFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenFire : public WorldGenerator {
public:
	WorldGenFire();
	virtual ~WorldGenFire();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

};

#endif // WORLDGENFIRE_H