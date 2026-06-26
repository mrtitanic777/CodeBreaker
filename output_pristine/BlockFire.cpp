#include "stdafx.h"
// Auto-generated from BlockFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFire.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockFire::BlockFire()
	: m_chanceToEncourageFire(new int[256])
	, m_abilityToCatchFire(new int[256])
{
}

BlockFire::BlockFire(int var1, int var2)
	: Block(51, 31, Material::s_fire)
	, m_chanceToEncourageFire(new int[256])
	, m_abilityToCatchFire(new int[256])
{
		setBurnRate(Block::s_planks->blockID, 5, 20);
		setBurnRate(Block::s_wood->blockID, 5, 5);
		setBurnRate(Block::s_leaves->blockID, 30, 60);
		setBurnRate(Block::s_bookShelf->blockID, 30, 20);
		setBurnRate(Block::s_tnt->blockID, 15, 100);

		for(var1 = 0; var1 < 16; ++var1) {
			setBurnRate(Block.clothRed.blockID + var1, 30, 60);
		}

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

AxisAlignedBB BlockFire::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return NULL;
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
		return 20;
}

void BlockFire::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		unsigned char var6 = var1->getBlockMetadata(var2, var3, var4);
		if(var6 < 15) {
			var1->setBlockMetadata(var2, var3, var4, var6 + 1);
			var1->scheduleBlockUpdate(var2, var3, var4, blockID);
		}

		if(!canNeighborCatchFire(var1, var2, var3, var4)) {
			if(!var1->isBlockNormalCube(var2, var3 - 1, var4) || var6 > 3) {
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}

		} else if(!canBlockCatchFire(var1, var2, var3 - 1, var4) && var6 == 15 && var5->nextInt(4) == 0) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
		} else {
			if(var6 % 5 == 0 && var6 > 5) {
				tryToCatchBlockOnFire(var1, var2 + 1, var3, var4, 300, var5);
				tryToCatchBlockOnFire(var1, var2 - 1, var3, var4, 300, var5);
				tryToCatchBlockOnFire(var1, var2, var3 - 1, var4, 100, var5);
				tryToCatchBlockOnFire(var1, var2, var3 + 1, var4, 200, var5);
				tryToCatchBlockOnFire(var1, var2, var3, var4 - 1, 300, var5);
				tryToCatchBlockOnFire(var1, var2, var3, var4 + 1, 300, var5);

				for(int var16 = var2 - 1; var16 <= var2 + 1; ++var16) {
					for(int var7 = var4 - 1; var7 <= var4 + 1; ++var7) {
						for(int var8 = var3 - 1; var8 <= var3 + 4; ++var8) {
							if(var16 != var2 || var8 != var3 || var7 != var4) {
								int var9 = 100;
								if(var8 > var3 + 1) {
									var9 = 100 + (var8 - (var3 + 1)) * 100;
								}

								int var10000;
								if(var1->getBlockId(var16, var8, var7) != 0) {
									var10000 = 0;
								} else {
									int var15 = getChanceToEncourageFire(var1, var16 + 1, var8, var7, 0);
									var15 = getChanceToEncourageFire(var1, var16 - 1, var8, var7, var15);
									var15 = getChanceToEncourageFire(var1, var16, var8 - 1, var7, var15);
									var15 = getChanceToEncourageFire(var1, var16, var8 + 1, var7, var15);
									var15 = getChanceToEncourageFire(var1, var16, var8, var7 - 1, var15);
									var15 = getChanceToEncourageFire(var1, var16, var8, var7 + 1, var15);
									var10000 = var15;
								}

								int var10 = var10000;
								if(var10 > 0 && var5->nextInt(var9) <= var10) {
									var1->setBlockWithNotify(var16, var8, var7, blockID);
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

bool BlockFire::canNeighborCatchFire(World* var1, int var2, int var3, int var4) {
		return canBlockCatchFire(var1, var2 + 1, var3, var4) ? true : (canBlockCatchFire(var1, var2 - 1, var3, var4) ? true : (canBlockCatchFire(var1, var2, var3 - 1, var4) ? true : (canBlockCatchFire(var1, var2, var3 + 1, var4) ? true : (canBlockCatchFire(var1, var2, var3, var4 - 1) ? true : canBlockCatchFire(var1, var2, var3, var4 + 1)))));
}

bool BlockFire::isCollidable() {
		return false;
}

bool BlockFire::canBlockCatchFire(World* var1, int var2, int var3, int var4) {
		return m_chanceToEncourageFire[var1->getBlockId(var2, var3, var4)] > 0;
}

int BlockFire::getChanceToEncourageFire(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = m_chanceToEncourageFire[var1->getBlockId(var2, var3, var4)];
		return var6 > var5 ? var6 : var5;
}

bool BlockFire::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockNormalCube(var2, var3 - 1, var4) || canNeighborCatchFire(var1, var2, var3, var4);
}

void BlockFire::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(!var1->isBlockNormalCube(var2, var3 - 1, var4) && !canNeighborCatchFire(var1, var2, var3, var4)) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}
}

void BlockFire::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(!var1->isBlockNormalCube(var2, var3 - 1, var4) && !canNeighborCatchFire(var1, var2, var3, var4)) {
			var1->setBlockWithNotify(var2, var3, var4, 0);
		} else {
			var1->scheduleBlockUpdate(var2, var3, var4, blockID);
		}
}

bool BlockFire::getChanceOfNeighborsEncouragingFire(int var1) {
		return m_chanceToEncourageFire[var1] > 0;
}

void BlockFire::fireSpread(World* var1, int var2, int var3, int var4) {
		bool var5 = false;
		var5 = fireCheck(var1, var2, var3 + 1, var4);
		if(!var5) {
			var5 = fireCheck(var1, var2 - 1, var3, var4);
		}

		if(!var5) {
			var5 = fireCheck(var1, var2 + 1, var3, var4);
		}

		if(!var5) {
			var5 = fireCheck(var1, var2, var3, var4 - 1);
		}

		if(!var5) {
			var5 = fireCheck(var1, var2, var3, var4 + 1);
		}

		if(!var5) {
			var5 = fireCheck(var1, var2, var3 - 1, var4);
		}

		if(!var5) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_fire->blockID);
		}

}

void BlockFire::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var5->nextInt(24) == 0) {
			var1->playSoundAtPlayer((float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F, "fire.fire", 1.0F + var5->nextFloat(), var5->nextFloat() * 0.7F + 0.3F);
		}

		int var6;
		float var7;
		float var8;
		float var9;
		if(!var1->isBlockNormalCube(var2, var3 - 1, var4) && !Block::s_fire->canBlockCatchFire(var1, var2, var3 - 1, var4)) {
			if(Block::s_fire->canBlockCatchFire(var1, var2 - 1, var3, var4)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat() * 0.1F;
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)var4 + var5->nextFloat();
					var1->spawnParticle("largesmoke", var7, var8, var9, 0.0F, 0.0F, 0.0F);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2 + 1, var3, var4)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)(var2 + 1) - var5->nextFloat() * 0.1F;
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)var4 + var5->nextFloat();
					var1->spawnParticle("largesmoke", var7, var8, var9, 0.0F, 0.0F, 0.0F);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2, var3, var4 - 1)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat();
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)var4 + var5->nextFloat() * 0.1F;
					var1->spawnParticle("largesmoke", var7, var8, var9, 0.0F, 0.0F, 0.0F);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2, var3, var4 + 1)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat();
					var8 = (float)var3 + var5->nextFloat();
					var9 = (float)(var4 + 1) - var5->nextFloat() * 0.1F;
					var1->spawnParticle("largesmoke", var7, var8, var9, 0.0F, 0.0F, 0.0F);
				}
			}

			if(Block::s_fire->canBlockCatchFire(var1, var2, var3 + 1, var4)) {
				for(var6 = 0; var6 < 2; ++var6) {
					var7 = (float)var2 + var5->nextFloat();
					var8 = (float)(var3 + 1) - var5->nextFloat() * 0.1F;
					var9 = (float)var4 + var5->nextFloat();
					var1->spawnParticle("largesmoke", var7, var8, var9, 0.0F, 0.0F, 0.0F);
				}
			}

		} else {
			for(var6 = 0; var6 < 3; ++var6) {
				var7 = (float)var2 + var5->nextFloat();
				var8 = (float)var3 + var5->nextFloat() * 0.5F + 0.5F;
				var9 = (float)var4 + var5->nextFloat();
				var1->spawnParticle("largesmoke", var7, var8, var9, 0.0F, 0.0F, 0.0F);
			}

		}
}

bool BlockFire::fireCheck(World* var0, int var1, int var2, int var3) {
		int var4 = var0->getBlockId(var1, var2, var3);
		if(var4 == Block::s_fire->blockID) {
			return true;
		} else if(var4 == 0) {
			var0->setBlockWithNotify(var1, var2, var3, Block::s_fire->blockID);
			return true;
		} else {
			return false;
		}
}
