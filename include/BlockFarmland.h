#ifndef BLOCKFARMLAND_H
#define BLOCKFARMLAND_H

// Auto-generated from BlockFarmland.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Random;
class World;

class BlockFarmland : public Block {
public:
	BlockFarmland();
	BlockFarmland(int var1);
	virtual ~BlockFarmland();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1);

protected:

};

#endif // BLOCKFARMLAND_H