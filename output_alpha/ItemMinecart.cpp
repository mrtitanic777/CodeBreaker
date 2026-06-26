#include "stdafx.h"
// Auto-generated from ItemMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemMinecart.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "Block.h"
#include "EntityMinecart.h"

ItemMinecart::ItemMinecart()
	: m_field_317_a(0)
{
}

ItemMinecart::ItemMinecart(int var1, int var2)
	: Item(var1)
	, m_field_317_a(0)
{
		maxStackSize = 1;
		m_field_317_a = var2;
}

ItemMinecart::~ItemMinecart() {
	// No dynamic resources
}

bool ItemMinecart::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		int var8 = var3->getBlockId(var4, var5, var6);
		if(var8 == Block::s_minecartTrack->blockID) {
			if(!var3->m_multiplayerWorld) {
				var3->entityJoinedWorld(new EntityMinecart(var3, (double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F), (double)((float)var6 + 0.5F), m_field_317_a));
			}

			--var1->stackSize;
			return true;
		} else {
			return false;
		}
}
