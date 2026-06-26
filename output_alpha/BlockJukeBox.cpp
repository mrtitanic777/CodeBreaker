#include "stdafx.h"
// Auto-generated from BlockJukeBox.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockJukeBox.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "EntityItem.h"
#include "ItemStack.h"
#include "Material.h"
#include "Item.h"

BlockJukeBox::BlockJukeBox()
{
}

BlockJukeBox::BlockJukeBox(int var1, int var2)
	: Block(var1, var2, Material::s_wood)
{
}

BlockJukeBox::~BlockJukeBox() {
	// No dynamic resources
}

int BlockJukeBox::getBlockTextureFromSide(int var1) {
		return blockIndexInTexture + (var1 == 1 ? 1 : 0);
}

bool BlockJukeBox::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if(var6 > 0) {
			ejectRecord(var1, var2, var3, var4, var6);
			return true;
		} else {
			return false;
		}
}

void BlockJukeBox::ejectRecord(World* var1, int var2, int var3, int var4, int var5) {
		var1->playRecord((std::string)NULL, var2, var3, var4);
		var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
		int var6 = Item::s_record13->shiftedIndex + var5 - 1;
		float var7 = 0.7F;
		double var8 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5f;
		double var10 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.2f + 0.6f;
		double var12 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5f;
		EntityItem* var14 = new EntityItem(var1, (double)var2 + var8, (double)var3 + var10, (double)var4 + var12, new ItemStack(var6));
		var14->m_field_805_c = 10;
		var1->entityJoinedWorld(var14);
}

void BlockJukeBox::dropBlockAsItemWithChance(World* var1, int var2, int var3, int var4, int var5, float var6) {
		if(!var1->m_multiplayerWorld) {
			if(var5 > 0) {
				ejectRecord(var1, var2, var3, var4, var5);
			}

			Block::dropBlockAsItemWithChance(var1, var2, var3, var4, var5, var6);
		}
}
