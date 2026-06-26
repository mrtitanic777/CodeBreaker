#ifndef WORLDGENLIGHTSTONE2_H
#define WORLDGENLIGHTSTONE2_H

// Auto-generated from WorldGenLightStone2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenLightStone2 : public WorldGenerator {
public:
	WorldGenLightStone2();
	virtual ~WorldGenLightStone2();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

};

#endif // WORLDGENLIGHTSTONE2_H