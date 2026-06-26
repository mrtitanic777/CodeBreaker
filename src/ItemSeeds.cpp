#include "stdafx.h"
// Auto-generated from ItemSeeds.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSeeds.h"
#include "Common.h"
#include "ItemStack.h"
#include "World.h"
#include "Block.h"
#include "Item.h"

ItemSeeds::ItemSeeds()
	: m_blockType(0)
{
}

ItemSeeds::ItemSeeds(int var1, int var2)
	: Item(39)
	, m_blockType(0)
{
		m_blockType = var2;
}

ItemSeeds::~ItemSeeds() {
	// No dynamic resources
}

bool ItemSeeds::onItemUse(const ItemStack& var1, World* var2, int var3, int var4, int var5, int var6) {
		if(var6 != 1) {
			return false;
		} else if(var3 > 0 && var4 > 0 && var5 > 0 && var3 < var2->width - 1 && var4 < var2->height - 1 && var5 < var2_count /* PORT: set this from allocation */ - 1) {
			var6 = var2->getBlockId(var3, var4, var5);
			if(var6 == Block::s_tilledField->blockID) {
				var2->setBlockWithNotify(var3, var4 + 1, var5, m_blockType);
				--var1.stackSize;
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
}
