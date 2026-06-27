#ifndef BLOCKSTATIONARY_H
#define BLOCKSTATIONARY_H

// Auto-generated from BlockStationary.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockFluids.h"

class Material;
class Random;
class World;

class BlockStationary : public BlockFluids {
public:
	BlockStationary(int var1, Material* var2);
	virtual ~BlockStationary();

	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void setNotStationary(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual bool isBlockBurning(World* var1, int var2, int var3, int var4);

	static void initStatics();
};

#endif // BLOCKSTATIONARY_H