#ifndef BLOCKCONTAINER_H
#define BLOCKCONTAINER_H

// Auto-generated from BlockContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Block.h"

class Material;
class TileEntity;
class World;

class BlockContainer : public Block {
public:
	BlockContainer();
	BlockContainer(int var1, Material* var2);
	virtual ~BlockContainer();

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);

public: // was protected
	virtual TileEntity* getBlockEntity() = 0;

};

#endif // BLOCKCONTAINER_H