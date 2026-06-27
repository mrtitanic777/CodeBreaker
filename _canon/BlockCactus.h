#ifndef BLOCKCACTUS_H
#define BLOCKCACTUS_H

// Auto-generated from BlockCactus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class AxisAlignedBB;
class Entity;
class Random;
class World;

class BlockCactus : public Block {
public:
	BlockCactus(int var1, int var2);
	virtual ~BlockCactus();

	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual AxisAlignedBB* getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual AxisAlignedBB* getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4);
	virtual int getBlockTextureFromSide(int var1);
	virtual bool renderAsNormalBlock();
	virtual bool isOpaqueCube();
	virtual int getRenderType();
	virtual bool canPlaceBlockAt(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canBlockStay(World* var1, int var2, int var3, int var4);
	virtual void onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5);

	static void initStatics();
};

#endif // BLOCKCACTUS_H