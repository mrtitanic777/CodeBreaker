#include "stdafx.h"
// Auto-generated from BlockBreakable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockBreakable.h"
#include "Common.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "Block.h"

BlockBreakable::BlockBreakable()
	: m_field_6363_a(false)
{
}

BlockBreakable::BlockBreakable(int var1, int var2, Material* var3, bool var4)
	: Block(var1, var2, var3)
	, m_field_6363_a(false)
{
		m_field_6363_a = var4;
}

BlockBreakable::~BlockBreakable() {
	// No dynamic resources
}

bool BlockBreakable::isOpaqueCube() {
		return false;
}

bool BlockBreakable::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockId(var2, var3, var4);
		return !m_field_6363_a && var6 == blockID ? false : Block::shouldSideBeRendered(var1, var2, var3, var4, var5);
}
