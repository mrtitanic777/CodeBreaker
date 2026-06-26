#ifndef BLOCKTORCH_H
#define BLOCKTORCH_H

// Auto-generated from BlockTorch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class MovingObjectPosition;
class Random;
class Vec3D;
class World;

class BlockTorch : public Block {
public:
	BlockTorch();
	BlockTorch(int var1, int var2);
	virtual ~BlockTorch();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlaced(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual MovingObjectPosition* collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

public: // was protected
public: // was private
	bool func_271_h(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKTORCH_H