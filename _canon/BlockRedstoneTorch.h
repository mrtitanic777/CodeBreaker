#ifndef BLOCKREDSTONETORCH_H
#define BLOCKREDSTONETORCH_H

// Auto-generated from BlockRedstoneTorch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockTorch.h"

class IBlockAccess;
class Random;
class World;

class BlockRedstoneTorch : public BlockTorch {
public:
	BlockRedstoneTorch(int var1, int var2, bool var3);
	virtual ~BlockRedstoneTorch();

	bool torchActive;
	static JList<void*> s_torchUpdates;

	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual bool checkForBurnout(World* var1, int var2, int var3, int var4, bool var5);
	virtual int tickRate();
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual bool isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual bool isAttachedBlockPowered(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1, Random* var2);
	virtual bool canProvidePower();
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

	static void initStatics();
};

#endif // BLOCKREDSTONETORCH_H