#ifndef BLOCKLEVER_H
#define BLOCKLEVER_H

// Auto-generated from BlockLever.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class EntityPlayer;
class IBlockAccess;
class World;

class BlockLever : public Block {
public:
	BlockLever(int var1, int var2);
	virtual ~BlockLever();

	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlaced(World* var1, int var2, int var3, int var4, int var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool checkIfAttachedToBlock(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual bool isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual bool isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canProvidePower();

	static void initStatics();
};

#endif // BLOCKLEVER_H