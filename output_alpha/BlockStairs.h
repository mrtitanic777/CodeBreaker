#ifndef BLOCKSTAIRS_H
#define BLOCKSTAIRS_H

// Auto-generated from BlockStairs.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"
#include <vector>

struct AxisAlignedBB;
class Entity;
class EntityLiving;
class EntityPlayer;
class IBlockAccess;
class Random;
class Vec3D;
class World;

class BlockStairs : public Block {
public:
	BlockStairs();
	BlockStairs(int var1, Block* var2);
	virtual ~BlockStairs();

	virtual void setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual void getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB var5, std::vector<AxisAlignedBB>& var6);  // PORT: list by reference
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5);
	virtual float getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4);
	virtual float func_227_a(Entity* var1);
	virtual int func_234_g();
	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual int getBlockTextureFromSide(int var1);
	virtual int getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual int tickRate();
	virtual AxisAlignedBB getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual void velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6);
	virtual bool isCollidable();
	virtual bool canCollideCheck(int var1, bool var2);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual void dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6);
	virtual void dropBlockAsItem(World* var1, int var2, int var3, int var4, int var5);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);
	virtual void onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4);
	virtual void onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5);

public: // was protected
public: // was private
	Block* m_modelBlock;

};

#endif // BLOCKSTAIRS_H