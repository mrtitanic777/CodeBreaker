#ifndef BLOCKMINECARTTRACK_H
#define BLOCKMINECARTTRACK_H

// Auto-generated from BlockMinecartTrack.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class IBlockAccess;
class MovingObjectPosition;
class Random;
class Vec3D;
class World;

class BlockMinecartTrack : public Block {
public:
	BlockMinecartTrack();
	BlockMinecartTrack(int var1, int var2);
	virtual ~BlockMinecartTrack();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual MovingObjectPosition* collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual int quantityDropped(Random* var1);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);

public: // was protected
public: // was private
	void func_4031_h(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKMINECARTTRACK_H