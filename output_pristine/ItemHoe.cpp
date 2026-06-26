#include "stdafx.h"
// Auto-generated from ItemHoe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemHoe.h"
#include "Common.h"
#include "ItemStack.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "EntityItem.h"
#include "Item.h"
#include "Material.h"
#include "StepSound.h"

ItemHoe::ItemHoe()
{
}

ItemHoe::ItemHoe(int var1, int var2)
	: Item(var1)
{
		maxStackSize = 1;
		maxDamage = 32 << var2;
}

ItemHoe::~ItemHoe() {
	// No dynamic resources
}

bool ItemHoe::onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6) {
		if(var3 > 0 && var4 > 0 && var5 > 0 && var3 < var2->width - 1 && var4 < var2->height - 1 && var5 < var2_count /* PORT: set this from allocation */ - 1) {
			var6 = var2->getBlockId(var3, var4, var5);
			Material var7 = var2->getBlockMaterial(var3, var4 + 1, var5);
			if((var7.isSolid() || var6 != Block::s_grass->blockID) && var6 != Block::s_dirt->blockID) {
				return false;
			} else {
				Block var12 = Block::s_tilledField;
				float var10001 = (float)var3 + 0.5F;
				float var10002 = (float)var4 + 0.5F;
				float var10003 = (float)var5 + 0.5F;
				std::string var10004 = var12.stepSound->stepSoundDir2();
				StepSound var8 = var12.stepSound;
				float var10005 = (var8.m_soundVolume + 1.0F) / 2.0F;
				var8 = var12.stepSound;
				var2->playSoundAtPlayer(var10001, var10002, var10003, var10004, var10005, var8.m_soundPitch * 0.8F);
				var2->setBlockWithNotify(var3, var4, var5, var12.blockID);
				var1->damageItem(1);
				if(var2->m_random->nextInt(8) == 0 && var6 == Block::s_grass->blockID) {
					for(int var9 = 0; var9 <= 0; ++var9) {
						float var10 = var2->m_random->nextFloat() * 0.7F + 0.15F;
						float var13 = var2->m_random->nextFloat() * 0.7F + 0.15F;
						EntityItem* var11 = new EntityItem(var2, (float)var3 + var10, (float)var4 + 1.2F, (float)var5 + var13, new ItemStack(Item::s_wheat));
						var11->m_delayBeforeCanPickup = 10;
						var2->spawnEntityInWorld(var11);
					}
				}

				return true;
			}
		} else {
			return false;
		}
}
