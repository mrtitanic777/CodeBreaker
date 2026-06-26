#ifndef BLOCKLADDER_H
#define BLOCKLADDER_H

// Auto-generated from BlockLadder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Random;
class World;

class BlockLadder : public Block {
public:
	BlockLadder();
	BlockLadder(int var1, int var2);
	virtual ~BlockLadder();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlaced(World* var1, int var2, int var3, int var4, int var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int quantityDropped(Random* var1);

public: // was protected
};

#endif // BLOCKLADDER_H