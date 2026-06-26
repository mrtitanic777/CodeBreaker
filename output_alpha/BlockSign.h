#ifndef BLOCKSIGN_H
#define BLOCKSIGN_H

// Auto-generated from BlockSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockContainer.h"

struct AxisAlignedBB;
class Class;
class IBlockAccess;
class Random;
class TileEntity;
class World;

class BlockSign : public BlockContainer {
public:
	BlockSign();
	BlockSign(int var1, void* var2, bool var3);
	virtual ~BlockSign();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual int getRenderType();
	virtual bool renderAsNormalBlock();
	virtual bool isOpaqueCube();
	virtual int idDropped(int var1, Random* var2);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);

public: // was protected
	virtual TileEntity* SetBlockEntity();

public: // was private
	void* m_signEntityClass;
	bool m_isFreestanding;

};

#endif // BLOCKSIGN_H