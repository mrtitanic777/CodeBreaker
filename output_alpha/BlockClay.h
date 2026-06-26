#ifndef BLOCKCLAY_H
#define BLOCKCLAY_H

// Auto-generated from BlockClay.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;

class BlockClay : public Block {
public:
	BlockClay();
	BlockClay(int var1, int var2);
	virtual ~BlockClay();

	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);

};

#endif // BLOCKCLAY_H