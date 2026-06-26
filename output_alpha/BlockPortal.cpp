#include "stdafx.h"
// Auto-generated from BlockPortal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockPortal.h"
#include "Common.h"
#include "BlockFire.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockPortal::BlockPortal()
{
}

BlockPortal::BlockPortal(int var1, int var2)
	: BlockBreakable(var1, var2, Material::s_field_4260_x, false)
{
}

BlockPortal::~BlockPortal() {
	// No dynamic resources
}

AxisAlignedBB BlockPortal::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

void BlockPortal::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		float var5;
		float var6;
		if(var1->getBlockId(var2 - 1, var3, var4) != blockID && var1->getBlockId(var2 + 1, var3, var4) != blockID) {
			var5 = 2.0F / 16.0F;
			var6 = 0.5F;
			setBlockBounds(0.5F - var5, 0.0F, 0.5F - var6, 0.5F + var5, 1.0F, 0.5F + var6);
		} else {
			var5 = 0.5F;
			var6 = 2.0F / 16.0F;
			setBlockBounds(0.5F - var5, 0.0F, 0.5F - var6, 0.5F + var5, 1.0F, 0.5F + var6);
		}

}

bool BlockPortal::isOpaqueCube() {
		return false;
}

bool BlockPortal::renderAsNormalBlock() {
		return false;
}

bool BlockPortal::tryToCreatePortal(World* var1, int var2, int var3, int var4) {
		unsigned char var5 = 0;
		unsigned char var6 = 0;
		if(var1->getBlockId(var2 - 1, var3, var4) == Block::s_obsidian->blockID || var1->getBlockId(var2 + 1, var3, var4) == Block::s_obsidian->blockID) {
			var5 = 1;
		}

		if(var1->getBlockId(var2, var3, var4 - 1) == Block::s_obsidian->blockID || var1->getBlockId(var2, var3, var4 + 1) == Block::s_obsidian->blockID) {
			var6 = 1;
		}

		DbgPrintF("%s\n", var5 + ", " + toStr(var6));
		if(var5 == var6) {
			return false;
		} else {
			if(var1->getBlockId(var2 - var5, var3, var4 - var6) == 0) {
				var2 -= var5;
				var4 -= var6;
			}

			int var7;
			int var8;
			for(var7 = -1; var7 <= 2; ++var7) {
				for(var8 = -1; var8 <= 3; ++var8) {
					bool var9 = var7 == -1 || var7 == 2 || var8 == -1 || var8 == 3;
					if(var7 != -1 && var7 != 2 || var8 != -1 && var8 != 3) {
						int var10 = var1->getBlockId(var2 + var5 * var7, var3 + var8, var4 + var6 * var7);
						if(var9) {
							if(var10 != Block::s_obsidian->blockID) {
								return false;
							}
						} else if(var10 != 0 && var10 != Block::s_fire->blockID) {
							return false;
						}
					}
				}
			}

			var1->m_field_1043_h = true;

			for(var7 = 0; var7 < 2; ++var7) {
				for(var8 = 0; var8 < 3; ++var8) {
					var1->setBlockWithNotify(var2 + var5 * var7, var3 + var8, var4 + var6 * var7, Block::s_portal->blockID);
				}
			}

			var1->m_field_1043_h = false;
			return true;
		}
}

void BlockPortal::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		unsigned char var6 = 0;
		unsigned char var7 = 1;
		if(var1->getBlockId(var2 - 1, var3, var4) == blockID || var1->getBlockId(var2 + 1, var3, var4) == blockID) {
			var6 = 1;
			var7 = 0;
		}

		int var8;
		for(var8 = var3; var1->getBlockId(var2, var8 - 1, var4) == blockID; --var8) {
		}

		if(var1->getBlockId(var2, var8 - 1, var4) != Block::s_obsidian->blockID) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
		} else {
			int var9;
			for(var9 = 1; var9 < 4 && var1->getBlockId(var2, var8 + var9, var4) == blockID; ++var9) {
			}

			if(var9 == 3 && var1->getBlockId(var2, var8 + var9, var4) == Block::s_obsidian->blockID) {
				bool var10 = var1->getBlockId(var2 - 1, var3, var4) == blockID || var1->getBlockId(var2 + 1, var3, var4) == blockID;
				bool var11 = var1->getBlockId(var2, var3, var4 - 1) == blockID || var1->getBlockId(var2, var3, var4 + 1) == blockID;
				if(var10 && var11) {
					var1->setBlockWithNotify(var2, var3, var4, 0);
				} else if((var1->getBlockId(var2 + var6, var3, var4 + var7) != Block::s_obsidian->blockID || var1->getBlockId(var2 - var6, var3, var4 - var7) != blockID) && (var1->getBlockId(var2 - var6, var3, var4 - var7) != Block::s_obsidian->blockID || var1->getBlockId(var2 + var6, var3, var4 + var7) != blockID)) {
					var1->setBlockWithNotify(var2, var3, var4, 0);
				}
			} else {
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}
		}
}

bool BlockPortal::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return true;
}

int BlockPortal::quantityDropped(Random* var1) {
		return 0;
}

int BlockPortal::func_234_g() {
		return 1;
}

void BlockPortal::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5) {
		if(!var1->m_multiplayerWorld) {
			var5->func_4039_q();
		}
}

void BlockPortal::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var5->nextInt(100) == 0) {
			var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "portal.portal", 1.0F, var5->nextFloat() * 0.4F + 0.8F);
		}

		for(int var6 = 0; var6 < 4; ++var6) {
			double var7 = (double)((float)var2 + var5->nextFloat());
			double var9 = (double)((float)var3 + var5->nextFloat());
			double var11 = (double)((float)var4 + var5->nextFloat());
			double var13 = 0.0f;
			double var15 = 0.0f;
			double var17 = 0.0f;
			int var19 = var5->nextInt(2) * 2 - 1;
			var13 = ((double)var5->nextFloat() - 0.5f) * 0.5f;
			var15 = ((double)var5->nextFloat() - 0.5f) * 0.5f;
			var17 = ((double)var5->nextFloat() - 0.5f) * 0.5f;
			if(var1->getBlockId(var2 - 1, var3, var4) != blockID && var1->getBlockId(var2 + 1, var3, var4) != blockID) {
				var7 = (double)var2 + 0.5f + 0.25f * (double)var19;
				var13 = (double)(var5->nextFloat() * 2.0F * (float)var19);
			} else {
				var11 = (double)var4 + 0.5f + 0.25f * (double)var19;
				var17 = (double)(var5->nextFloat() * 2.0F * (float)var19);
			}

			var1->spawnParticle("portal", var7, var9, var11, var13, var15, var17);
		}

}
