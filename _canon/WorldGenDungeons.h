#ifndef WORLDGENDUNGEONS_H
#define WORLDGENDUNGEONS_H

// Auto-generated from WorldGenDungeons.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldGenerator.h"

class ItemStack;
class Random;
class World;

class WorldGenDungeons : public WorldGenerator {
public:
	virtual ~WorldGenDungeons();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);
	virtual ItemStack* pickCheckLootItem(Random* var1);
	virtual JavaString pickMobSpawner(Random* var1);

	static void initStatics();
};

#endif // WORLDGENDUNGEONS_H