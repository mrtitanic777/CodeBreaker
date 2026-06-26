#include "stdafx.h"
// Auto-generated from BlockContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockContainer.h"
#include "Common.h"
#include "Material.h"
#include "TileEntity.h"
#include "World.h"
#include "Block.h"

BlockContainer::BlockContainer()
{
}

BlockContainer::BlockContainer(int var1, Material* var2)
	: Block(var1, var2)
{
}

BlockContainer::~BlockContainer() {
	// No dynamic resources
}

void BlockContainer::onBlockAdded(World* var1, int var2, int var3, int var4) {
		Block::onBlockAdded(var1, var2, var3, var4);
		var1->setBlockTileEntity(var2, var3, var4, getBlockEntity());
}

void BlockContainer::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		Block::onBlockRemoval(var1, var2, var3, var4);
		var1->removeBlockTileEntity(var2, var3, var4);
}
