#ifndef BLOCKLEAVES_H
#define BLOCKLEAVES_H

// Auto-generated from BlockLeaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockLeavesBase.h"

class Entity;
class IBlockAccess;
class Random;
class World;

class BlockLeaves : public BlockLeavesBase {
public:
	BlockLeaves(int var1, int var2);
	virtual ~BlockLeaves();

	int baseIndexInPNG;
	int leafDecayCounter;

	virtual int colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void propagateLeafDecay(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateLeafDecay(World* var1, int var2, int var3, int var4);
	virtual int getMaxNeighborDecayValue(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void removeLeafBlock(World* var1, int var2, int var3, int var4);
	virtual int quantityDropped(Random* var1);
	virtual int idDropped(int var1, Random* var2);
	virtual bool isOpaqueCube();
	virtual void setGraphicsLevel(bool var1);
	virtual void onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5);

	static void initStatics();
};

#endif // BLOCKLEAVES_H