#ifndef WORLDGENMINABLE_H
#define WORLDGENMINABLE_H

// Auto-generated from WorldGenMinable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenMinable : public WorldGenerator {
public:
	WorldGenMinable(int var1, int var2);
	virtual ~WorldGenMinable();

	int minableBlockId;
	int numberOfBlocks;

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENMINABLE_H