#ifndef BLOCKOREBLOCK_H
#define BLOCKOREBLOCK_H

// Auto-generated from BlockOreBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class BlockOreBlock : public Block {
public:
	BlockOreBlock(int var1, int var2);
	virtual ~BlockOreBlock();

	virtual int getBlockTextureFromSide(int var1);

	static void initStatics();
};

#endif // BLOCKOREBLOCK_H