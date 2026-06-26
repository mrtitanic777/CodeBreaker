#include "stdafx.h"
// Hand-fixed from ItemReed.java (Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemReed.h"
#include "Common.h"
#include "Block.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "StepSound.h"
#include "Item.h"

ItemReed::ItemReed()
	: m_field_320_a(0)
{
}

ItemReed::ItemReed(int var1, Block* var2)
	: Item(var1)
	, m_field_320_a(0)
{
	m_field_320_a = var2->blockID;
}

ItemReed::~ItemReed() {
	// No dynamic resources
}

bool ItemReed::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
	if(var3->getBlockId(var4, var5, var6) == Block::s_snow->blockID) {
		var7 = 0;
	} else {
		if(var7 == 0) {
			--var5;
		}

		if(var7 == 1) {
			++var5;
		}

		if(var7 == 2) {
			--var6;
		}

		if(var7 == 3) {
			++var6;
		}

		if(var7 == 4) {
			--var4;
		}

		if(var7 == 5) {
			++var4;
		}
	}

	if(var1->stackSize == 0) {
		return false;
	} else {
		if(var3->canBlockBePlacedAt(m_field_320_a, var4, var5, var6, false)) {
			Block* var8 = Block::blocksList[m_field_320_a];
			if(var3->setBlockWithNotify(var4, var5, var6, m_field_320_a)) {
				Block::blocksList[m_field_320_a]->onBlockPlaced(var3, var4, var5, var6, var7);
				var3->playSoundEffect((double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F), (double)((float)var6 + 0.5F), var8->stepSound->func_1145_d(), (var8->stepSound->func_1147_b() + 1.0F) / 2.0F, var8->stepSound->func_1144_c() * 0.8F);
				--var1->stackSize;
			}
		}

		return true;
	}
}
