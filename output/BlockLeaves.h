#ifndef BLOCKLEAVES_H
#define BLOCKLEAVES_H

// Auto-generated from BlockLeaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeavesBase.h"

class Random;

class BlockLeaves : public BlockLeavesBase {
public:
	BlockLeaves();
	BlockLeaves(int var1, int var2);
	virtual ~BlockLeaves();

	virtual int quantityDropped(Random* var1);
	virtual int idDropped(int var1);

public: // was protected
};

#endif // BLOCKLEAVES_H