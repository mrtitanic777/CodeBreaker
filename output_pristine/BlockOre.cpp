#include "stdafx.h"
// Auto-generated from BlockOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockOre.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "EntityItem.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"

BlockOre::BlockOre()
{
}

BlockOre::BlockOre(int var1, int var2)
	: Block(var1, var2, Material::s_rock)
{
}

BlockOre::~BlockOre() {
	// No dynamic resources
}

int BlockOre::idDropped(int var1) {
		return blockID == Block::s_oreCoal->blockID ? Item::s_coal->shiftedIndex : (blockID == Block::s_oreDiamond->blockID ? Item::s_diamond->shiftedIndex : blockID);
}

int BlockOre::quantityDropped(Random* var1) {
		return 1;
}

bool BlockOre::onBlockPlaced(World* var1, float var2, float var3, float var4) {
		int var5 = 0;
		if(blockID == Block::s_oreCoal->blockID) {
			var5 = Item::s_coal->shiftedIndex;
		}

		if(blockID == Block::s_oreDiamond->blockID) {
			var5 = Item::s_diamond->shiftedIndex;
		}

		if(blockID == Block::s_oreIron->blockID) {
			var5 = Item::s_ingotIron->shiftedIndex;
		}

		if(blockID == Block::s_oreGold->blockID) {
			var5 = Item::s_ingotGold->shiftedIndex;
		}

		for(int var6 = 0; var6 <= 0; ++var6) {
			if(var1->m_random->nextFloat() <= 1.0F) {
				float var7 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				float var8 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				float var9 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				EntityItem* var10 = new EntityItem(var1, var2 + var7, var3 + var8, var4 + var9, new ItemStack(var5));
				var10->m_delayBeforeCanPickup = 10;
				var1->spawnEntityInWorld(var10);
			}
		}

		return true;
}
