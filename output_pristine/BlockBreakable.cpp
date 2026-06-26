#include "stdafx.h"
// Auto-generated from BlockBreakable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBreakable.h"
#include "Common.h"
#include "Material.h"
#include "World.h"
#include "Block.h"

BlockBreakable::BlockBreakable()
	: m_localFlag(false)
{
}

BlockBreakable::BlockBreakable(int var1, int var2, Material* var3, bool var4)
	: Block(var1, var2, var3)
	, m_localFlag(false)
{
		m_localFlag = var4;
}

BlockBreakable::~BlockBreakable() {
	// No dynamic resources
}

bool BlockBreakable::isOpaqueCube() {
		return false;
}

bool BlockBreakable::shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockId(var2, var3, var4);
		return !m_localFlag && var6 == blockID ? false : Block::shouldSideBeRendered(var1, var2, var3, var4, var5);
}
