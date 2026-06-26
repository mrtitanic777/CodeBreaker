#include "stdafx.h"
// Hand-fixed from ItemHoe.java (Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemHoe.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Block.h"
#include "BlockGrass.h"
#include "Material.h"
#include "StepSound.h"
#include "EntityItem.h"
#include "Item.h"

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

bool ItemHoe::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
	int var8 = var3->getBlockId(var4, var5, var6);
	Material* var9 = var3->getBlockMaterial(var4, var5 + 1, var6);
	if((var9->func_878_a() || var8 != Block::s_grass->blockID) && var8 != Block::s_dirt->blockID) {
		return false;
	} else {
		Block* var10 = Block::s_tilledField;
		var3->playSoundEffect((double)((float)var4 + 0.5F), (double)((float)var5 + 0.5F), (double)((float)var6 + 0.5F), var10->stepSound->func_1145_d(), (var10->stepSound->func_1147_b() + 1.0F) / 2.0F, var10->stepSound->func_1144_c() * 0.8F);
		if(var3->m_multiplayerWorld) {
			return true;
		} else {
			var3->setBlockWithNotify(var4, var5, var6, var10->blockID);
			var1->damageItem(1);
			if(var3->rand->nextInt(8) == 0 && var8 == Block::s_grass->blockID) {
				unsigned char var11 = 1;

				for(int var12 = 0; var12 < var11; ++var12) {
					float var13 = 0.7F;
					float var14 = var3->rand->nextFloat() * var13 + (1.0F - var13) * 0.5F;
					float var15 = 1.2F;
					float var16 = var3->rand->nextFloat() * var13 + (1.0F - var13) * 0.5F;
					EntityItem* var17 = new EntityItem(var3, (double)((float)var4 + var14), (double)((float)var5 + var15), (double)((float)var6 + var16), new ItemStack(Item::s_seeds));
					var17->m_field_805_c = 10;
					var3->entityJoinedWorld(var17);
				}
			}

			return true;
		}
	}
}

bool ItemHoe::isFull3D() {
	return true;
}
