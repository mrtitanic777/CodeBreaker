#include "stdafx.h"
// Auto-generated from BlockFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFire.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"

BlockFire::BlockFire()
	: m_chanceToEncourageFire(new int[256])
	, m_abilityToCatchFire(new int[256])
{
}

BlockFire::BlockFire(int var1, int var2)
	: Block(var1, var2, Material::s_fire)
	, m_chanceToEncourageFire(new int[256])
	, m_abilityToCatchFire(new int[256])
{
		setBurnRate(Block::s_planks->blockID, 5, 20);
		setBurnRate(Block::s_wood->blockID, 5, 5);
		setBurnRate(Block::s_leaves->blockID, 30, 60);
		setBurnRate(Block::s_bookShelf->blockID, 30, 20);
		setBurnRate(Block::s_tnt->blockID, 15, 100);
		setBurnRate(Block::s_cloth->blockID, 30, 60);
		setTickOnLoad(true);
}

BlockFire::~BlockFire() {
	delete[] m_chanceToEncourageFire;
	m_chanceToEncourageFire = NULL;
	delete[] m_abilityToCatchFire;
	m_abilityToCatchFire = NULL;
}

void BlockFire::setBurnRate(int var1, int var2, int var3) {
		m_chanceToEncourageFire[var1] = var2;
		m_abilityToCatchFire[var1] = var3;
}

AxisAlignedBB BlockFire::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockFire::isOpaqueCube() {
		return false;
}

bool BlockFire::renderAsNormalBlock() {
		return false;
}

int BlockFire::getRenderType() {
		return 3;
}

int BlockFire::quantityDropped(Random* var1) {
		return 0;
}

int BlockFire::tickRate() {
		return 10;
}

void BlockFire::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		bool var6 = var1->getBlockId(var2, var3 - 1, var4) == Block::s_bloodStone->blockID;
		int var7 = var1->getBlockMetadata(var2, var3, var4);
		if(var7 < 15) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, var7 + 1);
			var1->scheduleBlockUpdate(var2, var3, var4, blockID);
		}

		if(!var6 && !func_263_h(var1, var2, var3, var4)) {
			if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4) || var7 > 3) {
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}

		} else if(!var6 && !canBlockCatchFire(var1, var2, var3 - 1, var4) && var7 == 15 && var5->nextInt(4) == 0) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
		} else {
			if(var7 % 2 == 0 && var7 > 2) {
				tryToCatchBlockOnFire(var1, var2 + 1, var3, var4, 300, var5);
				tryToCatchBlockOnFire(var1, var2 - 1, var3, var4, 300, var5);
				tryToCatchBlockOnFire(var1, var2, var3 - 1, var4, 250, var5);
				tryToCatchBlockOnFire(var1, var2, var3 + 1, var4, 250, var5);
				tryToCatchBlockOnFire(var1, var2, var3, var4 - 1, 300, var5);
				tryToCatchBlockOnFire(var1, var2, var3, var4 + 1, 300, var5);

				for(int var8 = var2 - 1; var8 <= var2 + 1; ++var8) {
					for(int var9 = var4 - 1; var9 <= var4 + 1; ++var9) {
						for(int var10 = var3 - 1; var10 <= var3 + 4; ++var10) {
							if(var8 != var2 || var10 != var3 || var9 != var4) {
								int var11 = 100;
								if(var10 > var3 + 1) {
									var11 += (var10 - (var3 + 1)) * 100;
								}

								int var12 = getChanceOfNeighborsEncouragingFire(var1, var8, var10, var9);
								if(var12 > 0 && var5->nextInt(var11) <= var12) {
									var1->setBlockWithNotify(var8, var10, var9, blockID);
								}
							}
						}
					}
				}
			}

		}
}

void BlockFire::tryToCatchBlockOnFire(World* var1, int var2, int var3, int var4, int var5, Random* var6) {
		int var7 = m_abilityToCatchFire[var1->getBlockId(var2, var3, var4)];
		if(var6->nextInt(var5) < var7) {
			bool var8 = var1->getBlockId(var2, var3, var4) == Block::s_tnt->blockID;
			if(var6->nextInt(2) == 0) {
				var1->setBlockWithNotify(var2, var3, var4, blockID);
			} else {
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}

			if(var8) {
				Block::s_tnt->onBlockDestroyedByPlayer(var1, var2, var3, var4, 0);
			}
		}

}

bool BlockFire::func_263_h(World* var1, int var2, int var3, int var4) {
		return canBlockCatchFire(var1, var2 + 1, var3, var4) ? true : (canBlockCatchFire(var1, var2 - 1, var3, var4) ? true : (canBlockCatchFire(var1, var2, var3 - 1, var4) ? true : (canBlockCatchFire(var1, var2, var3 + 1, var4) ? true : (canBlockCatchFire(var1, var2, var3, var4 - 1) ? true : canBlockCatchFire(var1, var2, var3, var4 + 1)))));
}

int BlockFire::getChanceOfNeighborsEncouragingFire(World* var1, int var2, int var3, int var4) {
		unsigned char var5 = 0;
		if(var1->getBlockId(var2, var3, var4) != 0) {
			return 0;
		} else {
			int var6 = getChanceToEncourageFire(var1, var2 + 1, var3, var4, var5);
			var6 = getChanceToEncourageFire(var1, var2 - 1, var3, var4, var6);
			var6 = getChanceToEncourageFire(var1, var2, var3 - 1, var4, var6);
			var6 = getChanceToEncourageFire(var1, var2, var3 + 1, var4, var6);
			var6 = getChanceToEncourageFire(var1, var2, var3, var4 - 1, var6);
			var6 = getChanceToEncourageFire(var1, var2, var3, var4 + 1, var6);
			return var6;
		}
}

bool BlockFire::isCollidable() {
		return false;
}

bool BlockFire::canBlockCatchFire(IBlockAccess* var1, int var2, int var3, int var4) {
		return m_chanceToEncourageFire[var1->getBlockId(var2, var3, var4)] > 0;
}

int BlockFire::getChanceToEncourageFire(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = m_chanceToEncourageFire[var1->getBlockId(var2, var3, var4)];
		return var6 > var5 ? var6 : var5;
}

bool BlockFire::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2, var3 - 1, var4) || func_263_h(var1, var2, var3, var4);
}

void BlockFire::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4) && !func_263_h(var1, var2, var3, var4)) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}
}

void BlockFire::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(var1->getBlockId(var2, var3 - 1, var4) != Block::s_obsidian->blockID || !Block::s_portal->tryToCreatePortal(var1, var2, var3, var4)) {
			if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4) && !func_263_h(var1, var2, var3, var4)) {
				var1->setBlockWithNotify(var2, var3, var4, 0);
			} else {
				var1->scheduleBlockUpdate(var2, var3, var4, blockID);
			}
		}
}

void BlockFire::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var5->nextInt(24) == 0) {
			var1->playSoundEffect((double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), "fire.fire", 1.0F + var5->nextFloat(), var5->nextFloat() * 0.7F + 0.3F);
		}

		int var6;
		float var7;
		float var8;
		float var9;
		if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4) && !Block::s_fire->canBlockCatchFire(var1, var2, var3 - 1, var4)) {
			if(Block::s_fire->canBlockCatchFire(var1, var2 - 1, var3, var4)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat() * 0.1F;
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)var4 + var5->nextFloat();
					var1->spawnParticle("largesmoke", (double)var7, (double)var8, (double)var9, 0.0f, 0.0f, 0.0f);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2 + 1, var3, var4)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)(var2 + 1) - var5->nextFloat() * 0.1F;
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)var4 + var5->nextFloat();
					var1->spawnParticle("largesmoke", (double)var7, (double)var8, (double)var9, 0.0f, 0.0f, 0.0f);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2, var3, var4 - 1)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat();
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)var4 + var5->nextFloat() * 0.1F;
					var1->spawnParticle("largesmoke", (double)var7, (double)var8, (double)var9, 0.0f, 0.0f, 0.0f);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2, var3, var4 + 1)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat();
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)(var4 + 1) - var5->nextFloat() * 0.1F;
					var1->spawnParticle("largesmoke", (double)var7, (double)var8, (double)var9, 0.0f, 0.0f, 0.0f);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2, var3 + 1, var4)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat();
					var8 = (float)(var3 + 1) - var5->nextFloat() * 0.1F;
					var9 = (float)var4 + var5->nextFloat();
					var1->spawnParticle("largesmoke", (double)var7, (double)var8, (double)var9, 0.0f, 0.0f, 0.0f);
				}
			}
		} else {
			for(var6 = 0; var6 < 3; ++var6) {
				var7 = (float)var2 + var5->nextFloat();
				var8 = (float)var3 + var5->nextFloat() * 0.5F + 0.5F;
				var9 = (float)var4 + var5->nextFloat();
				var1->spawnParticle("largesmoke", (double)var7, (double)var8, (double)var9, 0.0f, 0.0f, 0.0f);
			}
		}

}
