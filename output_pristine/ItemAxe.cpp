#include "stdafx.h"
// Auto-generated from ItemAxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemAxe.h"
#include "Common.h"
#include "Block.h"
#include "ItemTool.h"

Block** ItemAxe::s_blocksEffectiveAgainst = new Block[]{Block::s_planks, Block::s_bookShelf, Block::s_wood, Block::s_crate};

ItemAxe::ItemAxe()
{
}

ItemAxe::ItemAxe(int var1, int var2)
	: ItemTool(var1, 3, var2, m_blocksEffectiveAgainst)
{
}

ItemAxe::~ItemAxe() {
	// No dynamic resources
}
