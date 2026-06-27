#ifndef BLOCKFENCE_H
#define BLOCKFENCE_H

// Auto-generated from BlockFence.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class World;

class BlockFence : public Block {
public:
	BlockFence(int var1, int var2);
	virtual ~BlockFence();

	virtual void getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB* var5, JList<void*> var6);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();

	static void initStatics();
};

#endif // BLOCKFENCE_H