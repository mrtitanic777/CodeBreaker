#ifndef BLOCKSAPLING_H
#define BLOCKSAPLING_H

// Auto-generated from BlockSapling.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlower.h"

class Random;
class World;

class BlockSapling : public BlockFlower {
public:
	BlockSapling();
	BlockSapling(int var1, int var2);
	virtual ~BlockSapling();

	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);

protected:

};

#endif // BLOCKSAPLING_H