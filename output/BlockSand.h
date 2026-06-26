#ifndef BLOCKSAND_H
#define BLOCKSAND_H

// Auto-generated from BlockSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;
class World;

class BlockSand : public Block {
public:
	BlockSand();
	BlockSand(int var1, int var2);
	virtual ~BlockSand();

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool onBlockPlaced(World* var1, float var2, float var3, float var4);

public: // was private
	Random* rand;

	void tryToFall(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKSAND_H