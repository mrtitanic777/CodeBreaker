#ifndef WORLDGENHELLLAVA_H
#define WORLDGENHELLLAVA_H

// Auto-generated from WorldGenHellLava.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenHellLava : public WorldGenerator {
public:
	WorldGenHellLava(int var1);
	virtual ~WorldGenHellLava();

	int liquidBlockId;

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENHELLLAVA_H