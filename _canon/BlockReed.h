#ifndef BLOCKREED_H
#define BLOCKREED_H

// Auto-generated from BlockReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class Random;
class World;

class BlockReed : public Block {
public:
	BlockReed(int var1, int var2);
	virtual ~BlockReed();

	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void checkBlockCoordValid(World* var1, int var2, int var3, int var4);
	virtual bool canBlockStay(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual int idDropped(int var1, Random* var2);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();

	static void initStatics();
};

#endif // BLOCKREED_H