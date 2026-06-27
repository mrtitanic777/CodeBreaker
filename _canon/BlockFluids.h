#ifndef BLOCKFLUIDS_H
#define BLOCKFLUIDS_H

// Auto-generated from BlockFluids.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class Entity;
class IBlockAccess;
class Material;
class Random;
class Vec3D;
class World;

class BlockFluids : public Block {
public:
	BlockFluids(int var1, Material* var2);
	virtual ~BlockFluids();

	static float getFluidHeightPercent(int var0);
	virtual int getBlockTextureFromSide(int var1);
	virtual int getEffectiveFlowDecay(World* var1, int var2, int var3, int var4);
	virtual int getFluidLevel(IBlockAccess* var1, int var2, int var3, int var4);
	virtual bool renderAsNormalBlock();
	virtual bool isOpaqueCube();
	virtual bool canCollideCheck(int var1, bool var2);
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual int getRenderType();
	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual Vec3D* getFlowVector(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6);
	virtual int tickRate();
	virtual float getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int getRenderBlockPass();
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);
	static double getFlowDirection(IBlockAccess* var0, int var1, int var2, int var3, Material* var4);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void checkForHarden(World* var1, int var2, int var3, int var4);
	virtual void triggerLavaMixEffects(World* var1, int var2, int var3, int var4);

	static void initStatics();
};

#endif // BLOCKFLUIDS_H