#include "stdafx.h"
// Auto-generated from ItemPickaxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemPickaxe.h"
#include "Common.h"
#include "Block.h"
#include "ItemTool.h"
#include "Material.h"

Block* ItemPickaxe::s_blocksEffectiveAgainst = NULL /* PORT: Block array init {Block::s_cobblestone, Block::s_stairDouble, Block::s_stairSingle, Block::s_stone, Block.cobblestoneMossy, Block::s_oreIron, Block.blockSteel, Block::s_oreCoal, Block::s_blockGold, Block::s_oreGold, Block::s_oreDiamond, Block::s_blockDiamond} */;

ItemPickaxe::ItemPickaxe()
	: m_getHarvestLevel(0)
{
}

ItemPickaxe::ItemPickaxe(int var1, int var2)
	: ItemTool(var1, 2, var2, blocksEffectiveAgainst)
	, m_getHarvestLevel(0)
{
		m_getHarvestLevel = var2;
}

ItemPickaxe::~ItemPickaxe() {
	// No dynamic resources
}

bool ItemPickaxe::canHarvestBlock(Block* var1) {
		return var1 == Block::s_obsidian ? m_getHarvestLevel == 3 : (var1 != Block::s_blockDiamond && var1 != Block::s_oreDiamond ? (var1 != Block::s_blockGold && var1 != Block::s_oreGold ? (var1 != Block.blockSteel && var1 != Block::s_oreIron ? (var1->material == Material::s_rock ? true : var1->material == Material::s_iron) : m_getHarvestLevel > 0) : m_getHarvestLevel >= 2) : m_getHarvestLevel >= 2);
}
