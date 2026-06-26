#ifndef BLOCKMOBSPAWNER_H
#define BLOCKMOBSPAWNER_H

// Auto-generated from BlockMobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockContainer.h"

class Random;
class TileEntity;

class BlockMobSpawner : public BlockContainer {
public:
	BlockMobSpawner();
	BlockMobSpawner(int var1, int var2);
	virtual ~BlockMobSpawner();

	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual bool isOpaqueCube();

public: // was protected
	virtual TileEntity* SetBlockEntity();

};

#endif // BLOCKMOBSPAWNER_H