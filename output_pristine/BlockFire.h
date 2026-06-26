#ifndef BLOCKFIRE_H
#define BLOCKFIRE_H

// Auto-generated from BlockFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Random;
class World;

class BlockFire : public Block {
public:
	BlockFire();
	BlockFire(int var1, int var2);
	virtual ~BlockFire();

	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual bool renderAsNormalBlock();
	virtual int getRenderType();
	virtual int quantityDropped(Random* var1);
	virtual int tickRate();
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool isCollidable();
	virtual bool canBlockCatchFire(World* var1, int var2, int var3, int var4);
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual bool getChanceOfNeighborsEncouragingFire(int var1);
	virtual void fireSpread(World* var1, int var2, int var3, int var4);
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

protected:

private:
	int* m_chanceToEncourageFire;
	int* m_abilityToCatchFire;

	void setBurnRate(int var1, int var2, int var3);
	void tryToCatchBlockOnFire(World* var1, int var2, int var3, int var4, int var5, Random* var6);
	bool canNeighborCatchFire(World* var1, int var2, int var3, int var4);
	int getChanceToEncourageFire(World* var1, int var2, int var3, int var4, int var5);
	bool fireCheck(World* var0, int var1, int var2, int var3);

public:  // array-length companions
	int m_chanceToEncourageFire_count;
	int m_abilityToCatchFire_count;

};

#endif // BLOCKFIRE_H