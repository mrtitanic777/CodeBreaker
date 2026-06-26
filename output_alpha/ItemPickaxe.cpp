#include "stdafx.h"
// Auto-generated from ItemPickaxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemPickaxe.h"
#include "Common.h"
#include "Block.h"
#include "ItemTool.h"
#include "Material.h"

Block** ItemPickaxe::s_blocksEffectiveAgainst = NULL /* PORT: Block array init {Block::s_cobblestone, Block::s_stairDouble, Block::s_stairSingle, Block::s_stone, Block::s_cobblestoneMossy, Block::s_oreIron, Block::s_blockSteel, Block::s_oreCoal, Block::s_blockGold, Block::s_oreGold, Block::s_oreDiamond, Block::s_blockDiamond, Block::s_blockIce, Block::s_bloodStone} */;

ItemPickaxe::ItemPickaxe()
	: m_field_328_aY(0)
{
}

ItemPickaxe::ItemPickaxe(int var1, int var2)
	: ItemTool(var1, 2, var2, m_blocksEffectiveAgainst)
	, m_field_328_aY(0)
{
		m_field_328_aY = var2;
}

ItemPickaxe::~ItemPickaxe() {
	// No dynamic resources
}

bool ItemPickaxe::canHarvestBlock(Block* var1) {
		return var1 == Block::s_obsidian ? m_field_328_aY == 3 : (var1 != Block::s_blockDiamond && var1 != Block::s_oreDiamond ? (var1 != Block::s_blockGold && var1 != Block::s_oreGold ? (var1 != Block::s_blockSteel && var1 != Block::s_oreIron ? (var1 != Block::s_oreRedstone && var1 != Block::s_oreRedstoneGlowing ? (var1->m_blockMaterial == Material::s_rock ? true : var1->m_blockMaterial == Material::s_iron) : m_field_328_aY >= 2) : m_field_328_aY >= 1) : m_field_328_aY >= 2) : m_field_328_aY >= 2);
}
