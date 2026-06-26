#include "stdafx.h"
// Auto-generated from ItemSpade.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSpade.h"
#include "Common.h"
#include "Block.h"
#include "ItemTool.h"

Block** ItemSpade::s_blocksEffectiveAgainst = NULL /* PORT: Block array init {Block::s_grass, Block::s_dirt, Block::s_sand, Block::s_gravel, Block::s_snow, Block::s_blockSnow, Block::s_blockClay} */;

ItemSpade::ItemSpade()
{
}

ItemSpade::ItemSpade(int var1, int var2)
	: ItemTool(var1, 1, var2, m_blocksEffectiveAgainst)
{
}

ItemSpade::~ItemSpade() {
	// No dynamic resources
}

bool ItemSpade::canHarvestBlock(Block* var1) {
		return var1 == Block::s_snow ? true : var1 == Block::s_blockSnow;
}
