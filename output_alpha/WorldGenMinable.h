#ifndef WORLDGENMINABLE_H
#define WORLDGENMINABLE_H

// Auto-generated from WorldGenMinable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenMinable : public WorldGenerator {
public:
	WorldGenMinable();
	WorldGenMinable(int var1, int var2);
	virtual ~WorldGenMinable();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	int m_minableBlockId;
	int m_numberOfBlocks;

};

#endif // WORLDGENMINABLE_H