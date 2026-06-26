#ifndef BLOCKFLOWER_H
#define BLOCKFLOWER_H

// Auto-generated from BlockFlower.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Random;
class World;

class BlockFlower : public Block {
public:
	BlockFlower();
	BlockFlower(int var1, int var2);
	virtual ~BlockFlower();

	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool canBlockStay(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();

public: // was protected
	virtual bool canThisPlantGrowOnThisBlockID(int var1);

public: // was private
	void checkFlowerChange(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKFLOWER_H