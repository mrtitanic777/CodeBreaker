#ifndef BLOCKSTONE_H
#define BLOCKSTONE_H

// Auto-generated from BlockStone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Random;

class BlockStone : public Block {
public:
	BlockStone(int var1, int var2);
	virtual ~BlockStone();

	virtual int idDropped(int var1, Random* var2);

	static void initStatics();
};

#endif // BLOCKSTONE_H