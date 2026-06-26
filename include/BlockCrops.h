#ifndef BLOCKCROPS_H
#define BLOCKCROPS_H

// Auto-generated from BlockCrops.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlower.h"

class Random;
class World;

class BlockCrops : public BlockFlower {
public:
	BlockCrops();
	BlockCrops(int var1, int var2);
	virtual ~BlockCrops();

	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual int getRenderType();
	virtual void onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1);
	virtual int quantityDropped(Random* var1);

protected:
	virtual bool canThisPlantGrowOnThisBlockID(int var1);

};

#endif // BLOCKCROPS_H