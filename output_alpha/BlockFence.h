#ifndef BLOCKFENCE_H
#define BLOCKFENCE_H

// Auto-generated from BlockFence.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"
#include <vector>

struct AxisAlignedBB;
class World;

class BlockFence : public Block {
public:
	BlockFence();
	BlockFence(int var1, int var2);
	virtual ~BlockFence();

	virtual void getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB var5, std::vector<AxisAlignedBB>& var6);  // PORT: value-vector by reference
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();

};

#endif // BLOCKFENCE_H