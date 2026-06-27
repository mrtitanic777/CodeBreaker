#ifndef BLOCKOBSIDIAN_H
#define BLOCKOBSIDIAN_H

// Auto-generated from BlockObsidian.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "BlockStone.h"

class Random;

class BlockObsidian : public BlockStone {
public:
	BlockObsidian(int var1, int var2);
	virtual ~BlockObsidian();

	virtual int quantityDropped(Random* var1);
	virtual int idDropped(int var1, Random* var2);

	static void initStatics();
};

#endif // BLOCKOBSIDIAN_H