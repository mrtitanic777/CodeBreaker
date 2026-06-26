#ifndef BLOCKMUSHROOM_H
#define BLOCKMUSHROOM_H

// Auto-generated from BlockMushroom.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlower.h"

class World;

class BlockMushroom : public BlockFlower {
public:
	BlockMushroom();
	BlockMushroom(int var1, int var2);
	virtual ~BlockMushroom();

	virtual bool canBlockStay(World* var1, int var2, int var3, int var4);

public: // was protected
	virtual bool canThisPlantGrowOnThisBlockID(int var1);

};

#endif // BLOCKMUSHROOM_H