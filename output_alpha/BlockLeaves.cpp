#include "stdafx.h"
// Auto-generated from BlockLeaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeaves.h"
#include "Common.h"
#include "Entity.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "BlockLeavesBase.h"
#include "WorldChunkManager.h"

BlockLeaves::BlockLeaves()
	: m_baseIndexInPNG(0)
	, m_field_464_c(0)
{
}

BlockLeaves::BlockLeaves(int var1, int var2)
	: BlockLeavesBase(var1, var2, Material::s_leaves, false)
	, m_baseIndexInPNG(0)
	, m_field_464_c(0)
{
		m_baseIndexInPNG = var2;
}

BlockLeaves::~BlockLeaves() {
	// No dynamic resources
}

int BlockLeaves::colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4) {
		// PORT: Java used ColorizerFoliage.getFoliageColor(temperature, humidity)
		// which samples foliagecolor.png by biome temp/humidity. That colormap
		// asset is not available in this port, so we prime the chunk-manager
		// buffers and return the canonical default foliage tint.
		var1->func_4075_a()->func_4069_a(var2, var4, 1, 1);
		// double var5 = var1->func_4075_a()->m_temperature[0];
		// double var7 = var1->func_4075_a()->m_humidity[0];
		return 0x77AB2F;  // default foliage color (7842607)
}

void BlockLeaves::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(this == NULL) {
			m_field_464_c = 0;
			func_308_h(var1, var2, var3, var4);
			BlockLeavesBase::onNeighborBlockChange(var1, var2, var3, var4, var5);
		}
}

void BlockLeaves::func_6361_f(World* var1, int var2, int var3, int var4, int var5) {
		if(var1->getBlockId(var2, var3, var4) == blockID) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			if(var6 != 0 && var6 == var5 - 1) {
				func_308_h(var1, var2, var3, var4);
			}
		}
}

void BlockLeaves::func_308_h(World* var1, int var2, int var3, int var4) {
		if(this == NULL) {
			if(m_field_464_c++ < 100) {
				int var5 = var1->getBlockMaterial(var2, var3 - 1, var4)->func_878_a() ? 16 : 0;
				int var6 = var1->getBlockMetadata(var2, var3, var4);
				if(var6 == 0) {
					var6 = 1;
					var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
				}

				var5 = func_6362_g(var1, var2, var3 - 1, var4, var5);
				var5 = func_6362_g(var1, var2, var3, var4 - 1, var5);
				var5 = func_6362_g(var1, var2, var3, var4 + 1, var5);
				var5 = func_6362_g(var1, var2 - 1, var3, var4, var5);
				var5 = func_6362_g(var1, var2 + 1, var3, var4, var5);
				int var7 = var5 - 1;
				if(var7 < 10) {
					var7 = 1;
				}

				if(var7 != var6) {
					var1->setBlockMetadataWithNotify(var2, var3, var4, var7);
					func_6361_f(var1, var2, var3 - 1, var4, var6);
					func_6361_f(var1, var2, var3 + 1, var4, var6);
					func_6361_f(var1, var2, var3, var4 - 1, var6);
					func_6361_f(var1, var2, var3, var4 + 1, var6);
					func_6361_f(var1, var2 - 1, var3, var4, var6);
					func_6361_f(var1, var2 + 1, var3, var4, var6);
				}

			}
		}
}

int BlockLeaves::func_6362_g(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockId(var2, var3, var4);
		if(var6 == Block::s_wood->blockID) {
			return 16;
		} else {
			if(var6 == blockID) {
				int var7 = var1->getBlockMetadata(var2, var3, var4);
				if(var7 != 0 && var7 > var5) {
					return var7;
				}
			}

			return var5;
		}
}

void BlockLeaves::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(this == NULL) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			if(var6 == 0) {
				m_field_464_c = 0;
				func_308_h(var1, var2, var3, var4);
			} else if(var6 == 1) {
				func_6360_i(var1, var2, var3, var4);
			} else if(var5->nextInt(10) == 0) {
				func_308_h(var1, var2, var3, var4);
			}

		}
}

void BlockLeaves::func_6360_i(World* var1, int var2, int var3, int var4) {
		dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
		var1->setBlockWithNotify(var2, var3, var4, 0);
}

int BlockLeaves::quantityDropped(Random* var1) {
		return var1->nextInt(20) == 0 ? 1 : 0;
}

int BlockLeaves::idDropped(int var1, Random* var2) {
		return Block::s_sapling->blockID;
}

bool BlockLeaves::isOpaqueCube() {
		return !m_graphicsLevel;
}

void BlockLeaves::setGraphicsLevel(bool var1) {
		m_graphicsLevel = var1;
		blockIndexInTexture = m_baseIndexInPNG + (var1 ? 0 : 1);
}

void BlockLeaves::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5) {
		BlockLeavesBase::onEntityWalking(var1, var2, var3, var4, var5);
}
