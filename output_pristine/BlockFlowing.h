#ifndef BLOCKFLOWING_H
#define BLOCKFLOWING_H

// Auto-generated from BlockFlowing.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluid.h"

struct AxisAlignedBB;
class Material;
class Random;
class World;

class BlockFlowing : public BlockFluid {
public:
	BlockFlowing();
	BlockFlowing(int var1, Material* var2);
	virtual ~BlockFlowing();

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool update(World* var1, int var2, int var3, int var4, int var5);
	virtual bool shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5);
	virtual bool isCollidable();
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int tickRate();
	virtual int quantityDropped(Random* var1);
	virtual int getRenderBlockPass();

protected:

private:
	int m_stillId1;
	int m_movingId1;
	Random* rand;
	int* m_liquidIntArray;

	bool liquidSpread(World* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	bool liquidSpread2(World* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	bool extinguishFireLava(World* var0, int var1, int var2, int var3);
	bool flow(World* var0, int var1, int var2, int var3);

public:  // array-length companions
	int m_liquidIntArray_count;

};

#endif // BLOCKFLOWING_H