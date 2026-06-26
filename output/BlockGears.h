#ifndef BLOCKGEARS_H
#define BLOCKGEARS_H

// Auto-generated from BlockGears.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Random;

class BlockGears : public Block {
public:
	BlockGears();
	BlockGears(int var1, int var2);
	virtual ~BlockGears();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual int quantityDropped(Random* var1);
	virtual bool isCollidable();

public: // was protected
};

#endif // BLOCKGEARS_H