#include "stdafx.h"
// Auto-generated from ItemTool.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemTool.h"
#include "Common.h"
#include "Block.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "ItemStack.h"
#include "Item.h"

ItemTool::ItemTool()
	: m_blocksEffectiveAgainst(NULL)
	, m_efficiencyOnProperMaterial(4.0f)
	, m_damageVsEntity(0)
	, m_ingredientQuality(0)
{
}

ItemTool::ItemTool(int var1, int var2, int var3, Block** var4)
	: Item(var1)
	, m_blocksEffectiveAgainst(NULL)
	, m_efficiencyOnProperMaterial(4.0f)
	, m_damageVsEntity(0)
	, m_ingredientQuality(0)
{
		m_ingredientQuality = var3;
		m_blocksEffectiveAgainst = var4;
		maxStackSize = 1;
		maxDamage = 32 << var3;
		if(var3 == 3) {
			maxDamage *= 4;
		}

		m_efficiencyOnProperMaterial = (float)((var3 + 1) * 2);
		m_damageVsEntity = var2 + var3;
}

ItemTool::~ItemTool() {
	delete[] m_blocksEffectiveAgainst;
	m_blocksEffectiveAgainst = NULL;
}

float ItemTool::getStrVsBlock(ItemStack* var1, Block* var2) {
		for(int var3 = 0; var3 < m_blocksEffectiveAgainst_count /* PORT: set this from allocation */; ++var3) {
			if(m_blocksEffectiveAgainst[var3] == var2) {
				return m_efficiencyOnProperMaterial;
			}
		}

		return 1.0F;
}

void ItemTool::hitEntity(ItemStack* var1, EntityLiving* var2) {
		var1->damageItem(2);
}

void ItemTool::hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5) {
		var1->damageItem(1);
}

int ItemTool::getDamageVsEntity(Entity* var1) {
		return m_damageVsEntity;
}

bool ItemTool::isFull3D() {
		return true;
}
