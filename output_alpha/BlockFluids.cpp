#include "stdafx.h"
// Auto-generated from BlockFluids.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluids.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "IBlockAccess.h"
#include "Material.h"
#include "Random.h"
#include "Vec3D.h"
#include "World.h"
#include "Block.h"

BlockFluids::BlockFluids()
{
}

BlockFluids::BlockFluids(int var1, Material* var2)
	: Block(var1, (var2 == Material::s_lava ? 14 : 12) * 16 + 13, var2)
{
		float var3 = 0.0F;
		float var4 = 0.0F;
		setBlockBounds(0.0F + var4, 0.0F + var3, 0.0F + var4, 1.0F + var4, 1.0F + var3, 1.0F + var4);
		setTickOnLoad(true);
}

BlockFluids::~BlockFluids() {
	// No dynamic resources
}

float BlockFluids::func_288_b(int var0) {
		if(var0 >= 8) {
			var0 = 0;
		}

		float var1 = (float)(var0 + 1) / 9.0F;
		return var1;
}

int BlockFluids::getBlockTextureFromSide(int var1) {
		return var1 != 0 && var1 != 1 ? blockIndexInTexture + 1 : blockIndexInTexture;
}

int BlockFluids::func_290_h(World* var1, int var2, int var3, int var4) {
		return var1->getBlockMaterial(var2, var3, var4) != m_blockMaterial ? -1 : var1->getBlockMetadata(var2, var3, var4);
}

int BlockFluids::func_289_b(IBlockAccess* var1, int var2, int var3, int var4) {
		if(var1->getBlockMaterial(var2, var3, var4) != m_blockMaterial) {
			return -1;
		} else {
			int var5 = var1->getBlockMetadata(var2, var3, var4);
			if(var5 >= 8) {
				var5 = 0;
			}

			return var5;
		}
}

bool BlockFluids::renderAsNormalBlock() {
		return false;
}

bool BlockFluids::isOpaqueCube() {
		return false;
}

bool BlockFluids::canCollideCheck(int var1, bool var2) {
		return var2 && var1 == 0;
}

bool BlockFluids::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		Material* var6 = var1->getBlockMaterial(var2, var3, var4);
		return var6 == m_blockMaterial ? false : (var6 == Material::s_ice ? false : (var5 == 1 ? true : Block::shouldSideBeRendered(var1, var2, var3, var4, var5)));
}

AxisAlignedBB BlockFluids::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

int BlockFluids::getRenderType() {
		return 4;
}

int BlockFluids::idDropped(int var1, Random* var2) {
		return 0;
}

int BlockFluids::quantityDropped(Random* var1) {
		return 0;
}

Vec3D* BlockFluids::func_291_e(IBlockAccess* var1, int var2, int var3, int var4) {
		Vec3D* var5 = Vec3D::createVector(0.0f, 0.0f, 0.0f);
		int var6 = func_289_b(var1, var2, var3, var4);

		for(int var7 = 0; var7 < 4; ++var7) {
			int var8 = var2;
			int var10 = var4;
			if(var7 == 0) {
				var8 = var2 - 1;
			}

			if(var7 == 1) {
				var10 = var4 - 1;
			}

			if(var7 == 2) {
				++var8;
			}

			if(var7 == 3) {
				++var10;
			}

			int var11 = func_289_b(var1, var8, var3, var10);
			int var12;
			if(var11 < 0) {
				if(!var1->getBlockMaterial(var8, var3, var10)->func_880_c()) {
					var11 = func_289_b(var1, var8, var3 - 1, var10);
					if(var11 >= 0) {
						var12 = var11 - (var6 - 8);
						var5 = var5->addVector((double)((var8 - var2) * var12), (double)((var3 - var3) * var12), (double)((var10 - var4) * var12));
					}
				}
			} else if(var11 >= 0) {
				var12 = var11 - var6;
				var5 = var5->addVector((double)((var8 - var2) * var12), (double)((var3 - var3) * var12), (double)((var10 - var4) * var12));
			}
		}

		if(var1->getBlockMetadata(var2, var3, var4) >= 8) {
			bool var13 = false;
			if(var13 || shouldSideBeRendered(var1, var2, var3, var4 - 1, 2)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2, var3, var4 + 1, 3)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2 - 1, var3, var4, 4)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2 + 1, var3, var4, 5)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2, var3 + 1, var4 - 1, 2)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2, var3 + 1, var4 + 1, 3)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2 - 1, var3 + 1, var4, 4)) {
				var13 = true;
			}

			if(var13 || shouldSideBeRendered(var1, var2 + 1, var3 + 1, var4, 5)) {
				var13 = true;
			}

			if(var13) {
				var5 = var5->normalize()->addVector(0.0f, -6.0f, 0.0f);
			}
		}

		var5 = var5->normalize();
		return var5;
}

void BlockFluids::velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6) {
		Vec3D* var7 = func_291_e(var1, var2, var3, var4);
		var6->xCoord += var7->xCoord;
		var6->yCoord += var7->yCoord;
		var6->zCoord += var7->zCoord;
}

int BlockFluids::tickRate() {
		return m_blockMaterial == Material::s_water ? 5 : (m_blockMaterial == Material::s_lava ? 30 : 0);
}

float BlockFluids::getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4) {
		float var5 = var1->getLightBrightness(var2, var3, var4);
		float var6 = var1->getLightBrightness(var2, var3 + 1, var4);
		return var5 > var6 ? var5 : var6;
}

void BlockFluids::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		Block::updateTick(var1, var2, var3, var4, var5);
}

int BlockFluids::func_234_g() {
		return m_blockMaterial == Material::s_water ? 1 : 0;
}

void BlockFluids::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(m_blockMaterial == Material::s_water && var5->nextInt(64) == 0) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			if(var6 > 0 && var6 < 8) {
				var1->playSoundEffect((double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), "liquid.water", var5->nextFloat() * 0.25F + 12.0F / 16.0F, var5->nextFloat() * 1.0F + 0.5F);
			}
		}

		if(m_blockMaterial == Material::s_lava && var1->getBlockMaterial(var2, var3 + 1, var4) == Material::s_air && !var1->isBlockOpaqueCube(var2, var3 + 1, var4) && var5->nextInt(100) == 0) {
			double var12 = (double)((float)var2 + var5->nextFloat());
			double var8 = (double)var3 + maxY;
			double var10 = (double)((float)var4 + var5->nextFloat());
			var1->spawnParticle("lava", var12, var8, var10, 0.0f, 0.0f, 0.0f);
		}

}

double BlockFluids::func_293_a(IBlockAccess* var0, int var1, int var2, int var3, Material* var4) {
		Vec3D* var5 = NULL;
		if(var4 == Material::s_water) {
			var5 = ((BlockFluids*)Block::s_waterStill)->func_291_e(var0, var1, var2, var3);
		}

		if(var4 == Material::s_lava) {
			var5 = ((BlockFluids*)Block::s_lavaStill)->func_291_e(var0, var1, var2, var3);
		}

		return var5->xCoord == 0.0f && var5->zCoord == 0.0f ? -1000.0f : atan2f(var5->zCoord, var5->xCoord) - 3.14159265358979323846f * 0.5f;
}

void BlockFluids::onBlockAdded(World* var1, int var2, int var3, int var4) {
		checkForHarden(var1, var2, var3, var4);
}

void BlockFluids::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		checkForHarden(var1, var2, var3, var4);
}

void BlockFluids::checkForHarden(World* var1, int var2, int var3, int var4) {
		if(var1->getBlockId(var2, var3, var4) == blockID) {
			if(m_blockMaterial == Material::s_lava) {
				bool var5 = false;
				if(var5 || var1->getBlockMaterial(var2, var3, var4 - 1) == Material::s_water) {
					var5 = true;
				}

				if(var5 || var1->getBlockMaterial(var2, var3, var4 + 1) == Material::s_water) {
					var5 = true;
				}

				if(var5 || var1->getBlockMaterial(var2 - 1, var3, var4) == Material::s_water) {
					var5 = true;
				}

				if(var5 || var1->getBlockMaterial(var2 + 1, var3, var4) == Material::s_water) {
					var5 = true;
				}

				if(var5 || var1->getBlockMaterial(var2, var3 + 1, var4) == Material::s_water) {
					var5 = true;
				}

				if(var5) {
					int var6 = var1->getBlockMetadata(var2, var3, var4);
					if(var6 == 0) {
						var1->setBlockWithNotify(var2, var3, var4, Block::s_obsidian->blockID);
					} else if(var6 <= 4) {
						var1->setBlockWithNotify(var2, var3, var4, Block::s_cobblestone->blockID);
					}

					func_292_i(var1, var2, var3, var4);
				}
			}

		}
}

void BlockFluids::func_292_i(World* var1, int var2, int var3, int var4) {
		var1->playSoundEffect((double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), "random.fizz", 0.5F, 2.6F + (var1->rand->nextFloat() - var1->rand->nextFloat()) * 0.8F);

		for(int var5 = 0; var5 < 8; ++var5) {
			var1->spawnParticle("largesmoke", (double)var2 + ((float)::rand() / (float)RAND_MAX), (double)var3 + 1.2f, (double)var4 + ((float)::rand() / (float)RAND_MAX), 0.0f, 0.0f, 0.0f);
		}

}
