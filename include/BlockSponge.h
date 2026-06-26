#ifndef BLOCKSPONGE_H
#define BLOCKSPONGE_H

// Auto-generated from BlockSponge.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class World;

class BlockSponge : public Block {
public:
	BlockSponge();
	BlockSponge(int var1);
	virtual ~BlockSponge();

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);

protected:

};

#endif // BLOCKSPONGE_H