#ifndef BLOCKFLOWER_H
#define BLOCKFLOWER_H

// Auto-generated from BlockFlower.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class Random;
class World;

class BlockFlower : public Block {
public:
	BlockFlower(int var1, int var2);
	virtual ~BlockFlower();

	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual bool canThisPlantGrowOnThisBlockID(int var1);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void checkFlowerCanStay(World* var1, int var2, int var3, int var4);
	virtual bool canBlockStay(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();

	static void initStatics();
};

#endif // BLOCKFLOWER_H