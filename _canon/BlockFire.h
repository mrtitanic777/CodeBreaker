#ifndef BLOCKFIRE_H
#define BLOCKFIRE_H

// Auto-generated from BlockFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class IBlockAccess;
class Random;
class World;

class BlockFire : public Block {
public:
	BlockFire(int var1, int var2);
	virtual ~BlockFire();

	JArray<int > chanceToEncourageFire;
	JArray<int > abilityToCatchFire;

	virtual void setBurnRate(int var1, int var2, int var3);
	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual int quantityDropped(Random* var1);
	virtual int tickRate();
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void tryToCatchBlockOnFire(World* var1, int var2, int var3, int var4, int var5, Random* var6);
	virtual bool canNeighborBurn(World* var1, int var2, int var3, int var4);
	virtual int getChanceOfNeighborsEncouragingFire(World* var1, int var2, int var3, int var4);
	virtual bool isCollidable();
	virtual bool canBlockCatchFire(IBlockAccess* var1, int var2, int var3, int var4);
	virtual int getChanceToEncourageFire(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

	static void initStatics();
};

#endif // BLOCKFIRE_H