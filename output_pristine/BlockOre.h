#ifndef BLOCKORE_H
#define BLOCKORE_H

// Auto-generated from BlockOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;
class World;

class BlockOre : public Block {
public:
	BlockOre();
	BlockOre(int var1, int var2);
	virtual ~BlockOre();

	virtual int idDropped(int var1);
	virtual int quantityDropped(Random* var1);
	virtual bool onBlockPlaced(World* var1, float var2, float var3, float var4);

};

#endif // BLOCKORE_H