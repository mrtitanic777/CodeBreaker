#include "stdafx.h"
// Auto-generated from ItemTool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemTool.h"
#include "Common.h"
#include "Block.h"
#include "ItemStack.h"
#include "Item.h"

ItemTool::ItemTool()
	: m_blocksEffectiveAgainst(NULL)
	, m_efficiencyOnProperMaterial(4.0f)
	, m_damageVsEntity(0)
{
}

ItemTool::ItemTool(int var1, int var2, int var3, Block* var4)
	: Item(var1)
	, m_blocksEffectiveAgainst(NULL)
	, m_efficiencyOnProperMaterial(4.0f)
	, m_damageVsEntity(0)
{
		m_blocksEffectiveAgainst = var4;
		maxStackSize = 1;
		maxDamage = 32 << var3;
		m_efficiencyOnProperMaterial = (float)(var3 + 1 << 1);
		m_damageVsEntity = var2 + var3;
}

ItemTool::~ItemTool() {
	delete[] m_blocksEffectiveAgainst;
	m_blocksEffectiveAgainst = NULL;
}

float ItemTool::getStrVsBlock(Block* var1) {
		for(int var2 = 0; var2 < m_blocksEffectiveAgainst_count /* PORT: set this from allocation */; ++var2) {
			if(m_blocksEffectiveAgainst[var2] == var1) {
				return m_efficiencyOnProperMaterial;
			}
		}

		return 1.0F;
}

void ItemTool::hitEntity(const ItemStack& var1) {
		var1.damageItem(2);
}

void ItemTool::onBlockDestroyed(const ItemStack& var1) {
		var1.damageItem(1);
}

int ItemTool::getDamageVsEntity() {
		return m_damageVsEntity;
}
