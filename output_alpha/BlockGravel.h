#ifndef BLOCKGRAVEL_H
#define BLOCKGRAVEL_H

// Auto-generated from BlockGravel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSand.h"

class Random;

class BlockGravel : public BlockSand {
public:
	BlockGravel();
	BlockGravel(int var1, int var2);
	virtual ~BlockGravel();

	virtual int idDropped(int var1, Random* var2);

};

#endif // BLOCKGRAVEL_H