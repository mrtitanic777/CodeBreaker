#ifndef WORLDGENCLAY_H
#define WORLDGENCLAY_H

// Auto-generated from WorldGenClay.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenClay : public WorldGenerator {
public:
	WorldGenClay(int var1);
	virtual ~WorldGenClay();

	int clayBlockId;
	int numberOfBlocks;

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENCLAY_H