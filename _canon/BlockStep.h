#ifndef BLOCKSTEP_H
#define BLOCKSTEP_H

// Auto-generated from BlockStep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class IBlockAccess;
class Random;
class World;

class BlockStep : public Block {
public:
	BlockStep(int var1, bool var2);
	virtual ~BlockStep();

	bool isDoubleSlab;

	virtual int getBlockTextureFromSide(int var1);
	virtual bool isOpaqueCube();
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual int idDropped(int var1, Random* var2);
	virtual bool renderAsNormalBlock();
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // BLOCKSTEP_H