#ifndef BLOCKREDSTONEWIRE_H
#define BLOCKREDSTONEWIRE_H

// Auto-generated from BlockRedstoneWire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class IBlockAccess;
class Random;
class World;

class BlockRedstoneWire : public Block {
public:
	BlockRedstoneWire();
	BlockRedstoneWire(int var1, int var2);
	virtual ~BlockRedstoneWire();

	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1, Random* var2);
	virtual bool isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5);
	virtual bool isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual bool canProvidePower();
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);
	static bool isPowerProviderOrWire(IBlockAccess* var0, int var1, int var2, int var3);

public: // was private
	bool m_wiresProvidePower;

	void updateAndPropagateCurrentStrength(World* var1, int var2, int var3, int var4);
	void notifyWireNeighborsOfNeighborChange(World* var1, int var2, int var3, int var4);
	int getMaxCurrentStrength(World* var1, int var2, int var3, int var4, int var5);

};

#endif // BLOCKREDSTONEWIRE_H