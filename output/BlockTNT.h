#ifndef BLOCKTNT_H
#define BLOCKTNT_H

// Auto-generated from BlockTNT.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;
class World;

class BlockTNT : public Block {
public:
	BlockTNT();
	BlockTNT(int var1, int var2);
	virtual ~BlockTNT();

	virtual int getBlockTextureFromSide(int var1);
	virtual int quantityDropped(Random* var1);
	virtual void onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4);
	virtual void onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5);

};

#endif // BLOCKTNT_H