#ifndef BLOCKREDSTONETORCH_H
#define BLOCKREDSTONETORCH_H

// Auto-generated from BlockRedstoneTorch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockTorch.h"
#include <vector>

class IBlockAccess;
class Random;
class World;

class BlockRedstoneTorch : public BlockTorch {
public:
	BlockRedstoneTorch();
	BlockRedstoneTorch(int var1, int var2, bool var3);
	virtual ~BlockRedstoneTorch();

	virtual int getBlockTextureFromSideAndMetadata(int var1, int var2);
	virtual int tickRate();
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual bool isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual bool isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5);
	virtual int idDropped(int var1, Random* var2);
	virtual bool canProvidePower();
	virtual void randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5);

public: // was protected
public: // was private
	bool m_torchActive;
	static std::vector<void*> /* PORT: determine element type */ s_torchUpdates;

	bool checkForBurnout(World* var1, int var2, int var3, int var4, bool var5);
	bool func_15232_h(World* var1, int var2, int var3, int var4);

};

#endif // BLOCKREDSTONETORCH_H