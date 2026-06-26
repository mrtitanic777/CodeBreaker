#include "stdafx.h"
// Auto-generated from ItemSeeds.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSeeds.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Block.h"
#include "Item.h"

ItemSeeds::ItemSeeds()
	: m_field_318_a(0)
{
}

ItemSeeds::ItemSeeds(int var1, int var2)
	: Item(var1)
	, m_field_318_a(0)
{
		m_field_318_a = var2;
}

ItemSeeds::~ItemSeeds() {
	// No dynamic resources
}

bool ItemSeeds::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		if(var7 != 1) {
			return false;
		} else {
			int var8 = var3->getBlockId(var4, var5, var6);
			if(var8 == Block::s_tilledField->blockID) {
				var3->setBlockWithNotify(var4, var5 + 1, var6, m_field_318_a);
				--var1->stackSize;
				return true;
			} else {
				return false;
			}
		}
}
