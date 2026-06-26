#ifndef BLOCKSOURCE_H
#define BLOCKSOURCE_H

// Auto-generated from BlockSource.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Random;
class World;

class BlockSource : public Block {
public:
	BlockSource();
	BlockSource(int var1, int var2);
	virtual ~BlockSource();

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void updateTick(World* var1, int var2, int var3, int var4, Random* var5);

protected:

private:
	int m_fluid;

};

#endif // BLOCKSOURCE_H