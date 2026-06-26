#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H

// Auto-generated from BlockGrass.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;
class World;

class BlockGrass : public Block {
public:
	BlockGrass();
	BlockGrass(int var1);
	virtual ~BlockGrass();

	virtual int getBlockTextureFromSide(int var1);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);
	virtual int idDropped(int var1);

protected:

};

#endif // BLOCKGRASS_H