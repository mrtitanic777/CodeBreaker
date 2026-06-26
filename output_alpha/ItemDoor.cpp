#include "stdafx.h"
// Auto-generated from ItemDoor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemDoor.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "Material.h"
#include "World.h"
#include "Block.h"
#include "Item.h"

ItemDoor::ItemDoor()
	: m_field_321_a(NULL)
{
}

ItemDoor::ItemDoor(int var1, Material* var2)
	: Item(var1)
	, m_field_321_a(NULL)
{
		m_field_321_a = var2;
		maxDamage = 64;
		maxStackSize = 1;
}

ItemDoor::~ItemDoor() {
	m_field_321_a = NULL;
}

bool ItemDoor::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		if(var7 != 1) {
			return false;
		} else {
			++var5;
			Block* var8 = NULL;
			if(m_field_321_a == Material::s_wood) {
				var8 = Block::s_doorWood;
			} else {
				var8 = Block::s_doorSteel;
			}

			if(!var8->canPlaceBlockAt(var3, var4, var5, var6)) {
				return false;
			} else {
				int var9 = (int)floor((double)((var2->rotationYaw + 180.0F) * 4.0F / 360.0F) - 0.5f) & 3;
				unsigned char var10 = 0;
				unsigned char var11 = 0;
				if(var9 == 0) {
					var11 = 1;
				}

				if(var9 == 1) {
					var10 = -1;
				}

				if(var9 == 2) {
					var11 = -1;
				}

				if(var9 == 3) {
					var10 = 1;
				}

				int var12 = (var3->isBlockOpaqueCube(var4 - var10, var5, var6 - var11) ? 1 : 0) + (var3->isBlockOpaqueCube(var4 - var10, var5 + 1, var6 - var11) ? 1 : 0);
				int var13 = (var3->isBlockOpaqueCube(var4 + var10, var5, var6 + var11) ? 1 : 0) + (var3->isBlockOpaqueCube(var4 + var10, var5 + 1, var6 + var11) ? 1 : 0);
				bool var14 = var3->getBlockId(var4 - var10, var5, var6 - var11) == var8->blockID || var3->getBlockId(var4 - var10, var5 + 1, var6 - var11) == var8->blockID;
				bool var15 = var3->getBlockId(var4 + var10, var5, var6 + var11) == var8->blockID || var3->getBlockId(var4 + var10, var5 + 1, var6 + var11) == var8->blockID;
				bool var16 = false;
				if(var14 && !var15) {
					var16 = true;
				} else if(var13 > var12) {
					var16 = true;
				}

				if(var16) {
					var9 = var9 - 1 & 3;
					var9 += 4;
				}

				var3->setBlockWithNotify(var4, var5, var6, var8->blockID);
				var3->setBlockMetadataWithNotify(var4, var5, var6, var9);
				var3->setBlockWithNotify(var4, var5 + 1, var6, var8->blockID);
				var3->setBlockMetadataWithNotify(var4, var5 + 1, var6, var9 + 8);
				--var1->stackSize;
				return true;
			}
		}
}
