#ifndef BLOCKFLOWING_H
#define BLOCKFLOWING_H

// Auto-generated from BlockFlowing.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockFluids.h"

class Material;
class Random;
class World;

class BlockFlowing : public BlockFluids {
public:
	BlockFlowing(int var1, Material* var2);
	virtual ~BlockFlowing();

	int numAdjacentSources;
	JArray<bool > optimalFlowDirections;
	JArray<int > flowCost;

	virtual void setStationary(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void flowIntoBlock(World* var1, int var2, int var3, int var4, int var5);
	virtual int calculateFlowCost(World* var1, int var2, int var3, int var4, int var5, int var6);
	virtual JArray<bool > getOptimalFlowDirections(World* var1, int var2, int var3, int var4);
	virtual bool isBlockBlockingFlow(World* var1, int var2, int var3, int var4);
	virtual int getSmallestFlowDecay(World* var1, int var2, int var3, int var4, int var5);
	virtual bool canFlowInto(World* var1, int var2, int var3, int var4);
	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);

	static void initStatics();
};

#endif // BLOCKFLOWING_H