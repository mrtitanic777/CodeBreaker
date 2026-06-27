#ifndef BLOCKSAND_H
#define BLOCKSAND_H

// Auto-generated from BlockSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Random;
class World;

class BlockSand : public Block {
public:
	BlockSand(int var1, int var2);
	virtual ~BlockSand();

	static bool s_fallInstantly;

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual void tryToFall(World* var1, int var2, int var3, int var4);
	virtual int tickRate();
	static bool canFallBelow(World* var0, int var1, int var2, int var3);

	static void initStatics();
};

#endif // BLOCKSAND_H