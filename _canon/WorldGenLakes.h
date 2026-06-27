#ifndef WORLDGENLAKES_H
#define WORLDGENLAKES_H

// Auto-generated from WorldGenLakes.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenLakes : public WorldGenerator {
public:
	WorldGenLakes(int var1);
	virtual ~WorldGenLakes();

	int blockIndex;

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENLAKES_H