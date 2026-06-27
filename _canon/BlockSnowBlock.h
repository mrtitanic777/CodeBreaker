#ifndef BLOCKSNOWBLOCK_H
#define BLOCKSNOWBLOCK_H

// Auto-generated from BlockSnowBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Random;
class World;

class BlockSnowBlock : public Block {
public:
	BlockSnowBlock(int var1, int var2);
	virtual ~BlockSnowBlock();

	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);

	static void initStatics();
};

#endif // BLOCKSNOWBLOCK_H