#include "stdafx.h"
// Auto-generated from BlockLeavesBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeavesBase.h"
#include "Common.h"
#include "Material.h"
#include "World.h"
#include "Block.h"

BlockLeavesBase::BlockLeavesBase()
	: m_graphicsLevel(true)
{
}

BlockLeavesBase::BlockLeavesBase(int var1, int var2, Material* var3, bool var4)
	: Block(var1, var2, var3)
	, m_graphicsLevel(true)
{
}

BlockLeavesBase::~BlockLeavesBase() {
	// No dynamic resources
}

bool BlockLeavesBase::isOpaqueCube() {
		return false;
}

bool BlockLeavesBase::shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockId(var2, var3, var4);
		return !m_graphicsLevel && var6 == blockID ? false : Block::shouldSideBeRendered(var1, var2, var3, var4, var5);
}
