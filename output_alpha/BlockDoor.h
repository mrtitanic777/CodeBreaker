#ifndef BLOCKDOOR_H
#define BLOCKDOOR_H

// Auto-generated from BlockDoor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class EntityPlayer;
class IBlockAccess;
class Material;
class MovingObjectPosition;
class Random;
class Vec3D;
class World;

class BlockDoor : public Block {
public:
	BlockDoor();
	BlockDoor(int var1, Material* var2);
	virtual ~BlockDoor();

	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual AxisAlignedBB getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void func_313_b(int var1);
	virtual void onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void func_311_a(World* var1, int var2, int var3, int var4, bool var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1, Random* var2);
	virtual MovingObjectPosition* collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6);
	virtual int func_312_c(int var1);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);

public: // was protected
};

#endif // BLOCKDOOR_H