#ifndef WORLDGENDUNGEONS_H
#define WORLDGENDUNGEONS_H

// Auto-generated from WorldGenDungeons.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"
#include <string>

class ItemStack;
class Random;
class World;

class WorldGenDungeons : public WorldGenerator {
public:
	WorldGenDungeons();
	virtual ~WorldGenDungeons();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	ItemStack* pickCheckLootItem(Random* var1);
	std::string pickMobSpawner(Random* var1);

};

#endif // WORLDGENDUNGEONS_H