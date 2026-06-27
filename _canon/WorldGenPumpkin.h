#ifndef WORLDGENPUMPKIN_H
#define WORLDGENPUMPKIN_H

// Auto-generated from WorldGenPumpkin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class Random;
class World;

class WorldGenPumpkin : public WorldGenerator {
public:
	virtual ~WorldGenPumpkin();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDGENPUMPKIN_H