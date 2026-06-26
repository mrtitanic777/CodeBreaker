#ifndef WORLDGENREED_H
#define WORLDGENREED_H

// Auto-generated from WorldGenReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenReed : public WorldGenerator {
public:
	WorldGenReed();
	virtual ~WorldGenReed();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

};

#endif // WORLDGENREED_H