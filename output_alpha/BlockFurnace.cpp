#include "stdafx.h"
// Auto-generated from BlockFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFurnace.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "TileEntity.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "BlockContainer.h"
#include "TileEntityFurnace.h"

BlockFurnace::BlockFurnace()
	: m_isActive(false)
{
}

BlockFurnace::BlockFurnace(int var1, bool var2)
	: BlockContainer(var1, Material::s_rock)
	, m_isActive(false)
{
		m_isActive = var2;
		blockIndexInTexture = 45;
}

BlockFurnace::~BlockFurnace() {
	// No dynamic resources
}

int BlockFurnace::idDropped(int var1, Random* var2) {
		return Block::s_stoneOvenIdle->blockID;
}

void BlockFurnace::onBlockAdded(World* var1, int var2, int var3, int var4) {
		BlockContainer::onBlockAdded(var1, var2, var3, var4);
		func_284_h(var1, var2, var3, var4);
}

void BlockFurnace::func_284_h(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockId(var2, var3, var4 - 1);
		int var6 = var1->getBlockId(var2, var3, var4 + 1);
		int var7 = var1->getBlockId(var2 - 1, var3, var4);
		int var8 = var1->getBlockId(var2 + 1, var3, var4);
		unsigned char var9 = 3;
		if(Block::s_opaqueCubeLookup[var5] && !Block::s_opaqueCubeLookup[var6]) {
			var9 = 3;
		}

		if(Block::s_opaqueCubeLookup[var6] && !Block::s_opaqueCubeLookup[var5]) {
			var9 = 2;
		}

		if(Block::s_opaqueCubeLookup[var7] && !Block::s_opaqueCubeLookup[var8]) {
			var9 = 5;
		}

		if(Block::s_opaqueCubeLookup[var8] && !Block::s_opaqueCubeLookup[var7]) {
			var9 = 4;
		}

		var1->setBlockMetadataWithNotify(var2, var3, var4, var9);
}

int BlockFurnace::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		if(var5 == 1) {
			return Block::s_stone->blockIndexInTexture;
		} else if(var5 == 0) {
			return Block::s_stone->blockIndexInTexture;
		} else {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			return var5 != var6 ? blockIndexInTexture : (m_isActive ? blockIndexInTexture + 16 : blockIndexInTexture - 1);
		}
}

void BlockFurnace::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(m_isActive) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			float var7 = (float)var2 + 0.5F;
			float var8 = (float)var3 + 0.0F + var5->nextFloat() * 6.0F / 16.0F;
			float var9 = (float)var4 + 0.5F;
			float var10 = 0.52F;
			float var11 = var5->nextFloat() * 0.6F - 0.3F;
			if(var6 == 4) {
				var1->spawnParticle("smoke", (double)(var7 - var10), (double)var8, (double)(var9 + var11), 0.0f, 0.0f, 0.0f);
				var1->spawnParticle("flame", (double)(var7 - var10), (double)var8, (double)(var9 + var11), 0.0f, 0.0f, 0.0f);
			} else if(var6 == 5) {
				var1->spawnParticle("smoke", (double)(var7 + var10), (double)var8, (double)(var9 + var11), 0.0f, 0.0f, 0.0f);
				var1->spawnParticle("flame", (double)(var7 + var10), (double)var8, (double)(var9 + var11), 0.0f, 0.0f, 0.0f);
			} else if(var6 == 2) {
				var1->spawnParticle("smoke", (double)(var7 + var11), (double)var8, (double)(var9 - var10), 0.0f, 0.0f, 0.0f);
				var1->spawnParticle("flame", (double)(var7 + var11), (double)var8, (double)(var9 - var10), 0.0f, 0.0f, 0.0f);
			} else if(var6 == 3) {
				var1->spawnParticle("smoke", (double)(var7 + var11), (double)var8, (double)(var9 + var10), 0.0f, 0.0f, 0.0f);
				var1->spawnParticle("flame", (double)(var7 + var11), (double)var8, (double)(var9 + var10), 0.0f, 0.0f, 0.0f);
			}

		}
}

int BlockFurnace::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? Block::s_stone->blockID : (var1 == 0 ? Block::s_stone->blockID : (var1 == 3 ? blockIndexInTexture - 1 : blockIndexInTexture));
}

bool BlockFurnace::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		TileEntityFurnace* var6 = (TileEntityFurnace*)var1->getBlockTileEntity(var2, var3, var4);
		var5->displayGUIFurnace(var6);
		return true;
}

void BlockFurnace::updateFurnaceBlockState(bool var0, World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		TileEntity* var6 = var1->getBlockTileEntity(var2, var3, var4);
		if(var0) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_stoneOvenActive->blockID);
		} else {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_stoneOvenIdle->blockID);
		}

		var1->setBlockMetadataWithNotify(var2, var3, var4, var5);
		var1->setBlockTileEntity(var2, var3, var4, var6);
}

TileEntity* BlockFurnace::SetBlockEntity() {
		return new TileEntityFurnace();
}

void BlockFurnace::onBlockPlacedBy(World* var1, int var2, int var3, int var4, EntityLiving* var5) {
		int var6 = (int)floor((double)(var5->rotationYaw * 4.0F / 360.0F) + 0.5f) & 3;
		if(var6 == 0) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 2);
		}

		if(var6 == 1) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 5);
		}

		if(var6 == 2) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 3);
		}

		if(var6 == 3) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 4);
		}

}
