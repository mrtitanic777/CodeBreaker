#ifndef BLOCKSIGN_H
#define BLOCKSIGN_H

// Auto-generated from BlockSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockContainer.h"

class AxisAlignedBB;
class Class;
class IBlockAccess;
class Random;
class TileEntity;
class World;

class BlockSign : public BlockContainer {
public:
	BlockSign(int var1, Class* var2, bool var3);
	virtual ~BlockSign();

	Class* signEntityClass;
	bool isFreestanding;

	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB* getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual int getRenderType();
	virtual bool renderAsNormalBlock();
	virtual bool isOpaqueCube();
	virtual TileEntity* SetBlockEntity();
	virtual int idDropped(int var1, Random* var2);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // BLOCKSIGN_H