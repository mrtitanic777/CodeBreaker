#ifndef BLOCKPRESSUREPLATE_H
#define BLOCKPRESSUREPLATE_H

// Auto-generated from BlockPressurePlate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"
#include "EnumMobType.h"

class AxisAlignedBB;
class Entity;
class IBlockAccess;
class Random;
class World;

class BlockPressurePlate : public Block {
public:
	BlockPressurePlate(int var1, int var2, int var3);
	virtual ~BlockPressurePlate();

	int triggerMobType;

	virtual int tickRate();
	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5);
	virtual void setStateIfMobInteractsWithPlate(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual bool isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual bool isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canProvidePower();
	virtual void setBlockBoundsForItemRender();

	static void initStatics();
};

#endif // BLOCKPRESSUREPLATE_H