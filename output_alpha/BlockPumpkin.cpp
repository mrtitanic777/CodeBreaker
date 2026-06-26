#include "stdafx.h"
// Auto-generated from BlockPumpkin.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockPumpkin.h"
#include "Common.h"
#include "EntityLiving.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockPumpkin::BlockPumpkin()
	: m_field_4072_a(false)
{
}

BlockPumpkin::BlockPumpkin(int var1, int var2, bool var3)
	: Block(var1, Material::s_pumpkin)
	, m_field_4072_a(false)
{
		blockIndexInTexture = var2;
		setTickOnLoad(true);
		m_field_4072_a = var3;
}

BlockPumpkin::~BlockPumpkin() {
	// No dynamic resources
}

int BlockPumpkin::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		if(var1 == 1) {
			return blockIndexInTexture;
		} else if(var1 == 0) {
			return blockIndexInTexture;
		} else {
			int var3 = blockIndexInTexture + 1 + 16;
			if(m_field_4072_a) {
				++var3;
			}

			return var2 == 0 && var1 == 2 ? var3 : (var2 == 1 && var1 == 5 ? var3 : (var2 == 2 && var1 == 3 ? var3 : (var2 == 3 && var1 == 4 ? var3 : blockIndexInTexture + 16)));
		}
}

int BlockPumpkin::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? blockIndexInTexture : (var1 == 0 ? blockIndexInTexture : (var1 == 3 ? blockIndexInTexture + 1 + 16 : blockIndexInTexture + 16));
}

void BlockPumpkin::onBlockAdded(World* var1, int var2, int var3, int var4) {
		Block::onBlockAdded(var1, var2, var3, var4);
}

bool BlockPumpkin::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockId(var2, var3, var4);
		return (var5 == 0 || Block::blocksList[var5]->m_blockMaterial->getIsLiquid()) && var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockPumpkin::onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5) {
		int var6 = (int)floor((double)(var5->rotationYaw * 4.0F / 360.0F) + 0.5f) & 3;
		var1->setBlockMetadataWithNotify(var2, var3, var4, var6);
}
