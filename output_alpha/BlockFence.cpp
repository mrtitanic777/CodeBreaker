#include "stdafx.h"
// Auto-generated from BlockFence.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFence.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockFence::BlockFence()
{
}

BlockFence::BlockFence(int var1, int var2)
	: Block(var1, var2, Material::s_wood)
{
}

BlockFence::~BlockFence() {
	// No dynamic resources
}

void BlockFence::getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB var5, std::vector<AxisAlignedBB>& var6) {
		var6.push_back(AxisAlignedBB::getBoundingBoxFromPool((double)var2, (double)var3, (double)var4, (double)(var2 + 1), (double)var3 + 1.5f, (double)(var4 + 1)));
}

bool BlockFence::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->getBlockId(var2, var3 - 1, var4) == blockID ? false : (!var1->getBlockMaterial(var2, var3 - 1, var4)->func_878_a() ? false : Block::canPlaceBlockAt(var1, var2, var3, var4));
}

bool BlockFence::isOpaqueCube() {
		return false;
}

bool BlockFence::renderAsNormalBlock() {
		return false;
}

int BlockFence::getRenderType() {
		return 11;
}
