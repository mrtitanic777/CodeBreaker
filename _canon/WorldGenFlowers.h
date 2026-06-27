#ifndef WORLDGENFLOWERS_H
#define WORLDGENFLOWERS_H

// Auto-generated from WorldGenFlowers.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenFlowers : public WorldGenerator {
public:
	WorldGenFlowers(int var1);
	virtual ~WorldGenFlowers();

	int plantBlockId;

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENFLOWERS_H