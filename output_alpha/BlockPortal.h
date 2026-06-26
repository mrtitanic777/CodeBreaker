#ifndef BLOCKPORTAL_H
#define BLOCKPORTAL_H

// Auto-generated from BlockPortal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBreakable.h"

struct AxisAlignedBB;
class Entity;
class IBlockAccess;
class Random;
class World;

class BlockPortal : public BlockBreakable {
public:
	BlockPortal();
	BlockPortal(int var1, int var2);
	virtual ~BlockPortal();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual bool tryToCreatePortal(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual int quantityDropped(Random* var1);
	virtual int func_234_g();
	virtual void onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

};

#endif // BLOCKPORTAL_H