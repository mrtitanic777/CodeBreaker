#ifndef WORLDGENTREES_H
#define WORLDGENTREES_H

// Auto-generated from WorldGenTrees.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenTrees : public WorldGenerator {
public:
	virtual ~WorldGenTrees();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENTREES_H