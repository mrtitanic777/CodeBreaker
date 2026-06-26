#ifndef BLOCKWORKBENCH_H
#define BLOCKWORKBENCH_H

// Auto-generated from BlockWorkbench.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class EntityPlayer;
class World;

class BlockWorkbench : public Block {
public:
	BlockWorkbench();
	BlockWorkbench(int var1);
	virtual ~BlockWorkbench();

	virtual int getBlockTextureFromSide(int var1);
	virtual bool blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5);

public: // was protected
};

#endif // BLOCKWORKBENCH_H