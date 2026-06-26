#ifndef WORLDGENFLOWERS_H
#define WORLDGENFLOWERS_H

// Auto-generated from WorldGenFlowers.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenFlowers : public WorldGenerator {
public:
	WorldGenFlowers();
	WorldGenFlowers(int var1);
	virtual ~WorldGenFlowers();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	int m_plantBlockId;

};

#endif // WORLDGENFLOWERS_H