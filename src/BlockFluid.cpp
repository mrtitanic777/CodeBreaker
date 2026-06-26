#include "stdafx.h"
// Auto-generated from BlockFluid.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluid.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Material.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "BlockFire.h"

BlockFluid::BlockFluid()
	: m_stillId(0)
	, m_movingId(0)
{
}

BlockFluid::BlockFluid(int var1, Material* var2)
	: Block(var1, var2)
	, m_stillId(0)
	, m_movingId(0)
{
		blockIndexInTexture = 14;
		if(var2 == Material::s_lava) {
			blockIndexInTexture = 30;
		}

		Block.isBlockFluid[var1] = true;
		m_movingId = var1;
		m_stillId = var1 + 1;
		setBlockBounds(0.01F, -0.09F, 0.01F, 1.01F, 0.90999997F, 1.01F);
		setTickOnLoad(true);
		setResistance(2.0F);
}

BlockFluid::~BlockFluid() {
	// No dynamic resources
}

int BlockFluid::getBlockTextureFromSide(int var1) {
		return material == Material::s_lava ? blockIndexInTexture : (var1 == 1 ? blockIndexInTexture : (var1 == 0 ? blockIndexInTexture : blockIndexInTexture + 32));
}

bool BlockFluid::renderAsNormalBlock() {
		return false;
}

void BlockFluid::onBlockAdded(World* var1, int var2, int var3, int var4) {
		var1->scheduleBlockUpdate(var2, var3, var4, m_movingId);
}

void BlockFluid::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		update(var1, var2, var3, var4, 0);
}

bool BlockFluid::update(World* var1, int var2, int var3, int var4, int var5) {
		bool var7 = false;

		bool var6;
		do {
			--var3;
			if(!canFlow(var1, var2, var3, var4)) {
				break;
			}

			var6 = var1->setBlockWithNotify(var2, var3, var4, m_movingId);
			if(var6) {
				var7 = true;
			}
		} while(var6 && material != Material::s_lava);

		++var3;
		if(material == Material::s_water || !var7) {
			var7 |= flow(var1, var2 - 1, var3, var4);
			var7 |= flow(var1, var2 + 1, var3, var4);
			var7 |= flow(var1, var2, var3, var4 - 1);
			var7 |= flow(var1, var2, var3, var4 + 1);
		}

		if(material == Material::s_lava) {
			var7 |= extinguishFireLava(var1, var2 - 1, var3, var4);
			var7 |= extinguishFireLava(var1, var2 + 1, var3, var4);
			var7 |= extinguishFireLava(var1, var2, var3, var4 - 1);
			var7 |= extinguishFireLava(var1, var2, var3, var4 + 1);
		}

		if(!var7) {
			var1->setTileNoUpdate(var2, var3, var4, m_stillId);
		} else {
			var1->scheduleBlockUpdate(var2, var3, var4, m_movingId);
		}

		return var7;
}

bool BlockFluid::canFlow(World* var1, int var2, int var3, int var4) {
		if(!var1->getBlockMaterial(var2, var3, var4).liquidSolidCheck()) {
			return false;
		} else {
			if(material == Material::s_water) {
				for(int var5 = var2 - 2; var5 <= var2 + 2; ++var5) {
					for(int var6 = var3 - 2; var6 <= var3 + 2; ++var6) {
						for(int var7 = var4 - 2; var7 <= var4 + 2; ++var7) {
							if(var1->getBlockId(var5, var6, var7) == Block::s_sponge->blockID) {
								return false;
							}
						}
					}
				}
			}

			return true;
		}
}

bool BlockFluid::extinguishFireLava(World* var0, int var1, int var2, int var3) {
		if(Block::s_fire->getChanceOfNeighborsEncouragingFire(var0->getBlockId(var1, var2, var3))) {
			Block::s_fire->fireSpread(var0, var1, var2, var3);
			return true;
		} else {
			return false;
		}
}

bool BlockFluid::flow(World* var1, int var2, int var3, int var4) {
		if(!canFlow(var1, var2, var3, var4)) {
			return false;
		} else {
			bool var5 = var1->setBlockWithNotify(var2, var3, var4, m_movingId);
			if(var5) {
				var1->scheduleBlockUpdate(var2, var3, var4, m_movingId);
			}

			return false;
		}
}

float BlockFluid::getBlockBrightness(World* var1, int var2, int var3, int var4) {
		return material == Material::s_lava ? 100.0F : var1->getLightBrightness(var2, var3, var4);
}

bool BlockFluid::shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5) {
		if(var2 >= 0 && var3 >= 0 && var4 >= 0 && var2 < var1->width && var4 < var1_count /* PORT: set this from allocation */) {
			int var6 = var1->getBlockId(var2, var3, var4);
			return var6 != m_movingId && var6 != m_stillId ? (var5 != 1 || var1->getBlockId(var2 - 1, var3, var4) != 0 && var1->getBlockId(var2 + 1, var3, var4) != 0 && var1->getBlockId(var2, var3, var4 - 1) != 0 && var1->getBlockId(var2, var3, var4 + 1) != 0 ? Block::shouldSideBeRendered(var1, var2, var3, var4, var5) : true) : false;
		} else {
			return false;
		}
}

bool BlockFluid::isCollidable() {
		return false;
}

AxisAlignedBB BlockFluid::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return AxisAlignedBB();
}

bool BlockFluid::isOpaqueCube() {
		return false;
}

void BlockFluid::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(var5 != 0) {
			Material* var6 = Block::blocksList[var5].material;
			if(material == Material::s_water && var6 == Material::s_lava || var6 == Material::s_water && material == Material::s_lava) {
				var1->setBlockWithNotify(var2, var3, var4, Block::s_stone->blockID);
			}
		}

		var1->scheduleBlockUpdate(var2, var3, var4, blockID);
}

int BlockFluid::tickRate() {
		return material == Material::s_lava ? 25 : 5;
}

int BlockFluid::quantityDropped(Random* var1) {
		return 0;
}

int BlockFluid::getRenderBlockPass() {
		return material == Material::s_water ? 1 : 0;
}

void BlockFluid::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var5->nextInt(128) == -1 && var1->getBlockMaterial(var2, var3 + 1, var4).getIsSolid()) {
			if(material == Material::s_lava) {
				var1->playSoundAtPlayer((float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F, "liquid.lava", var5->nextFloat() * 0.25F + 12.0F / 16.0F, var5->nextFloat() * 0.5F + 0.3F);
			}

			if(material == Material::s_water) {
				var1->playSoundAtPlayer((float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F, "liquid.water", var5->nextFloat() * 0.25F + 12.0F / 16.0F, var5->nextFloat() + 0.5F);
			}
		}

		if(material == Material::s_lava && var1->getBlockMaterial(var2, var3 + 1, var4) == Material::s_air && !var1->isBlockNormalCube(var2, var3 + 1, var4) && var5->nextInt(100) == 0) {
			float var6 = (float)var2 + var5->nextFloat();
			float var7 = (float)var3 + maxY;
			float var8 = (float)var4 + var5->nextFloat();
			var1->spawnParticle("lava", var6, var7, var8, 0.0F, 0.0F, 0.0F);
		}

		if(material == Material::s_water) {
			int var9;
			if(liquidAirCheck(var1, var2 + 1, var3, var4)) {
				for(var9 = 0; var9 < 4; ++var9) {
					var1->spawnParticle("splash", (float)(var2 + 1) + 2.0F / 16.0F, (float)var3, (float)var4 + var5->nextFloat(), 0.0F, 0.0F, 0.0F);
				}
			}

			if(liquidAirCheck(var1, var2 - 1, var3, var4)) {
				for(var9 = 0; var9 < 4; ++var9) {
					var1->spawnParticle("splash", (float)var2 - 2.0F / 16.0F, (float)var3, (float)var4 + var5->nextFloat(), 0.0F, 0.0F, 0.0F);
				}
			}

			if(liquidAirCheck(var1, var2, var3, var4 + 1)) {
				for(var9 = 0; var9 < 4; ++var9) {
					var1->spawnParticle("splash", (float)var2 + var5->nextFloat(), (float)var3, (float)(var4 + 1) + 2.0F / 16.0F, 0.0F, 0.0F, 0.0F);
				}
			}

			if(liquidAirCheck(var1, var2, var3, var4 - 1)) {
				for(var9 = 0; var9 < 4; ++var9) {
					var1->spawnParticle("splash", (float)var2 + var5->nextFloat(), (float)var3, (float)var4 - 2.0F / 16.0F, 0.0F, 0.0F, 0.0F);
				}
			}
		}

}

bool BlockFluid::liquidAirCheck(World* var0, int var1, int var2, int var3) {
		Material* var4 = var0->getBlockMaterial(var1, var2, var3);
		Material* var5 = var0->getBlockMaterial(var1, var2 - 1, var3);
		return !var4->getIsSolid() && !var4->getIsLiquid() ? var5->getIsSolid() || var5->getIsLiquid() : false;
}
