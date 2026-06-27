#ifndef BLOCKCONTAINER_H
#define BLOCKCONTAINER_H

// Auto-generated from BlockContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Block.h"

class Material;
class TileEntity;
class World;

class BlockContainer : public Block {
public:
	BlockContainer(int var1, Material* var2);
	BlockContainer(int var1, int var2, Material* var3);
	virtual ~BlockContainer();

	virtual void onBlockAdded(World* var1, int var2, int var3, int var4);
	virtual void onBlockRemoval(World* var1, int var2, int var3, int var4);
	virtual TileEntity* SetBlockEntity() = 0;

	static void initStatics();
};

#endif // BLOCKCONTAINER_H