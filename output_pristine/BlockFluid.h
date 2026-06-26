#ifndef BLOCKFLUID_H
#define BLOCKFLUID_H

// Auto-generated from BlockFluid.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

struct AxisAlignedBB;
class Material;
class Random;
class World;

class BlockFluid : public Block {
public:
	BlockFluid();
	BlockFluid(int var1, Material* var2);
	virtual ~BlockFluid();

	virtual int getBlockTextureFromSide(int var1);
	virtual bool renderAsNormalBlock();
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool update(World* var1, int var2, int var3, int var4, int var5);
	virtual float getBlockBrightness(World* var1, int var2, int var3, int var4);
	virtual bool shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5);
	virtual bool isCollidable();
	virtual AxisAlignedBB getCollisionBoundingBoxFromPool(int var1, int var2, int var3);
	virtual bool isOpaqueCube();
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual int tickRate();
	virtual int quantityDropped(Random* var1);
	virtual int getRenderBlockPass();
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

protected:
	int m_stillId;
	int m_movingId;

	virtual bool canFlow(World* var1, int var2, int var3, int var4);

private:
	bool extinguishFireLava(World* var0, int var1, int var2, int var3);
	bool flow(World* var1, int var2, int var3, int var4);
	bool liquidAirCheck(World* var0, int var1, int var2, int var3);

};

#endif // BLOCKFLUID_H