#ifndef BLOCKICE_H
#define BLOCKICE_H

// Auto-generated from BlockIce.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBreakable.h"

class IBlockAccess;
class Random;
class World;

class BlockIce : public BlockBreakable {
public:
	BlockIce();
	BlockIce(int var1, int var2);
	virtual ~BlockIce();

	virtual int func_234_g();
	virtual bool shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual int quantityDropped(Random* var1);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);

};

#endif // BLOCKICE_H