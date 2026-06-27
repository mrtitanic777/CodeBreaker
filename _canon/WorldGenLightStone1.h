#ifndef WORLDGENLIGHTSTONE1_H
#define WORLDGENLIGHTSTONE1_H

// Auto-generated from WorldGenLightStone1.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenLightStone1 : public WorldGenerator {
public:
	virtual ~WorldGenLightStone1();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENLIGHTSTONE1_H