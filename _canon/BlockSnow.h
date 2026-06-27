#ifndef BLOCKSNOW_H
#define BLOCKSNOW_H

// Auto-generated from BlockSnow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class IBlockAccess;
class Random;
class World;

class BlockSnow : public Block {
public:
	BlockSnow(int var1, int var2);
	virtual ~BlockSnow();

	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canSnowStay(World* var1, int var2, int var3, int var4);
	virtual void harvestBlock(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // BLOCKSNOW_H