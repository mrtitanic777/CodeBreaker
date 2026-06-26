#ifndef BLOCKORE_H
#define BLOCKORE_H

// Auto-generated from BlockOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;

class BlockOre : public Block {
public:
	BlockOre();
	BlockOre(int var1, int var2);
	virtual ~BlockOre();

	virtual int idDropped(int var1, Random* var2);
	virtual int quantityDropped(Random* var1);

};

#endif // BLOCKORE_H