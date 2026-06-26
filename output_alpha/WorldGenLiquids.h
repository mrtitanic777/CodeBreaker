#ifndef WORLDGENLIQUIDS_H
#define WORLDGENLIQUIDS_H

// Auto-generated from WorldGenLiquids.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenLiquids : public WorldGenerator {
public:
	WorldGenLiquids();
	WorldGenLiquids(int var1);
	virtual ~WorldGenLiquids();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	int m_liquidBlockId;

};

#endif // WORLDGENLIQUIDS_H