#ifndef WORLDGENTREES_H
#define WORLDGENTREES_H

// Auto-generated from WorldGenTrees.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenTrees : public WorldGenerator {
public:
	WorldGenTrees();
	virtual ~WorldGenTrees();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

};

#endif // WORLDGENTREES_H