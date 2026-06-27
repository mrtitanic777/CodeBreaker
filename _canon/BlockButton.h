#ifndef BLOCKBUTTON_H
#define BLOCKBUTTON_H

// Auto-generated from BlockButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class EntityPlayer;
class IBlockAccess;
class Random;
class World;

class BlockButton : public Block {
public:
	BlockButton(int var1, int var2);
	virtual ~BlockButton();

	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual int tickRate();
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlaced(World* var1, int var2, int var3, int var4, int var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool checkIfAttachedValid(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual bool isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual bool isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canProvidePower();
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void setBlockBoundsForItemRender();

	static void initStatics();
};

#endif // BLOCKBUTTON_H