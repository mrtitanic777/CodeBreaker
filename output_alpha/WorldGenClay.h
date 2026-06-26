#ifndef WORLDGENCLAY_H
#define WORLDGENCLAY_H

// Auto-generated from WorldGenClay.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenerator.h"

class Random;
class World;

class WorldGenClay : public WorldGenerator {
public:
	WorldGenClay();
	WorldGenClay(int var1);
	virtual ~WorldGenClay();

	virtual bool generate(World* var1, Random* var2, int var3, int var4, int var5);

public: // was private
	int m_clayBlockId;
	int m_numberOfBlocks;

};

#endif // WORLDGENCLAY_H