#ifndef WORLDGENCACTUS_H
#define WORLDGENCACTUS_H

// Auto-generated from WorldGenCactus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenCactus : public WorldGenerator {
public:
	WorldGenCactus();
	virtual ~WorldGenCactus();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

};

#endif // WORLDGENCACTUS_H