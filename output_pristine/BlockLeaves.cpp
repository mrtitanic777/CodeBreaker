#include "stdafx.h"
// Auto-generated from BlockLeaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeaves.h"
#include "Common.h"
#include "Random.h"
#include "Block.h"
#include "Material.h"

BlockLeaves::BlockLeaves()
{
}

BlockLeaves::BlockLeaves(int var1, int var2)
	: BlockLeavesBase(18, 52, Material::s_leaves, true)
{
}

BlockLeaves::~BlockLeaves() {
	// No dynamic resources
}

int BlockLeaves::quantityDropped(Random* var1) {
		return var1->nextInt(6) == 0 ? 1 : 0;
}

int BlockLeaves::idDropped(int var1) {
		return Block::s_sapling->blockID;
}
