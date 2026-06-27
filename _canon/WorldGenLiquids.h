#ifndef WORLDGENLIQUIDS_H
#define WORLDGENLIQUIDS_H

// Auto-generated from WorldGenLiquids.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenLiquids : public WorldGenerator {
public:
	WorldGenLiquids(int var1);
	virtual ~WorldGenLiquids();

	int liquidBlockId;

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENLIQUIDS_H