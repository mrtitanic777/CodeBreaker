#include "stdafx.h"
// Auto-generated from BlockFlowing.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlowing.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Material.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "BlockFire.h"

BlockFlowing::BlockFlowing()
	: m_stillId1(0)
	, m_movingId1(0)
	, rand(new Random())
	, m_liquidIntArray(NULL /* PORT: int array init {0, 1, 2, 3} */)
{
}

BlockFlowing::BlockFlowing(int var1, Material* var2)
	: BlockFluid(var1, var2)
	, m_stillId1(0)
	, m_movingId1(0)
	, rand(new Random())
	, m_liquidIntArray(NULL /* PORT: int array init {0, 1, 2, 3} */)
{
		blockIndexInTexture = 14;
		if(var2 == Material::s_lava) {
			blockIndexInTexture = 30;
		}

		Block.isBlockFluid[var1] = true;
		m_movingId1 = var1;
		m_stillId1 = var1 + 1;
		setBlockBounds(0.01F, -0.09F, 0.01F, 1.01F, 0.90999997F, 1.01F);
		setTickOnLoad(true);
}

BlockFlowing::~BlockFlowing() {
	rand = NULL;
	delete[] m_liquidIntArray;
	m_liquidIntArray = NULL;
}

void BlockFlowing::onBlockAdded(World* var1, int var2, int var3, int var4) {
		var1->scheduleBlockUpdate(var2, var3, var4, m_movingId1);
}

void BlockFlowing::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		update(var1, var2, var3, var4, 0);
}

bool BlockFlowing::update(World* var1, int var2, int var3, int var4, int var5) {
		bool var10 = false;
		bool var6 = canFlow(var1, var2 - 1, var3, var4) || canFlow(var1, var2 + 1, var3, var4) || canFlow(var1, var2, var3, var4 - 1) || canFlow(var1, var2, var3, var4 + 1);
		int var7;
		int var8;
		int var9;
		if(var6 && var1->getBlockMaterial(var2, var3 - 1, var4) == material) {
			var5 = var1->floodFill(var2, var3 - 1, var4, m_movingId1, m_stillId1);
			if(var5 <= 0) {
				return false;
			}

			if(var5 == 2) {
				var7 = var1->fluidFlowCheck(var2, var3, var4, m_movingId1, m_stillId1);
				if(var7 != -9999) {
					if(var7 < 0) {
						return false;
					}

					var8 = var7 % 1024;
					var7 >>= 10;
					var9 = var7 % 1024;
					var7 >>= 10;
					var2 = var7 % 1024;
					var1->setBlockWithNotify(var8, var2, var9, 0);
					return false;
				}

				return false;
			}
		}

		var10 = liquidSpread2(var1, var2, var3, var4, var2, var3 - 1, var4);

		for(var7 = 0; var7 < 4; ++var7) {
			var8 = rand->nextInt(4 - var7) + var7;
			var9 = m_liquidIntArray[var7];
			m_liquidIntArray[var7] = m_liquidIntArray[var8];
			m_liquidIntArray[var8] = var9;
			if(m_liquidIntArray[var7] == 0 && !var10) {
				var10 = liquidSpread2(var1, var2, var3, var4, var2 - 1, var3, var4);
			}

			if(m_liquidIntArray[var7] == 1 && !var10) {
				var10 = liquidSpread2(var1, var2, var3, var4, var2 + 1, var3, var4);
			}

			if(m_liquidIntArray[var7] == 2 && !var10) {
				var10 = liquidSpread2(var1, var2, var3, var4, var2, var3, var4 - 1);
			}

			if(m_liquidIntArray[var7] == 3 && !var10) {
				var10 = liquidSpread2(var1, var2, var3, var4, var2, var3, var4 + 1);
			}
		}

		if(!var10 && var6) {
			if(rand->nextInt(3) == 0) {
				if(rand->nextInt(3) == 0) {
					var10 = false;

					for(var7 = 0; var7 < 4; ++var7) {
						var8 = rand->nextInt(4 - var7) + var7;
						var9 = m_liquidIntArray[var7];
						m_liquidIntArray[var7] = m_liquidIntArray[var8];
						m_liquidIntArray[var8] = var9;
						if(m_liquidIntArray[var7] == 0 && !var10) {
							var10 = liquidSpread(var1, var2, var3, var4, var2 - 1, var3, var4);
						}

						if(m_liquidIntArray[var7] == 1 && !var10) {
							var10 = liquidSpread(var1, var2, var3, var4, var2 + 1, var3, var4);
						}

						if(m_liquidIntArray[var7] == 2 && !var10) {
							var10 = liquidSpread(var1, var2, var3, var4, var2, var3, var4 - 1);
						}

						if(m_liquidIntArray[var7] == 3 && !var10) {
							var10 = liquidSpread(var1, var2, var3, var4, var2, var3, var4 + 1);
						}
					}
				} else if(material == Material::s_lava) {
					var1->setBlockWithNotify(var2, var3, var4, Block::s_stone->blockID);
				} else {
					var1->setBlockWithNotify(var2, var3, var4, 0);
				}
			}

			return false;
		} else {
			if(material == Material::s_water) {
				var10 |= extinguishFireLava(var1, var2 - 1, var3, var4);
				var10 |= extinguishFireLava(var1, var2 + 1, var3, var4);
				var10 |= extinguishFireLava(var1, var2, var3, var4 - 1);
				var10 |= extinguishFireLava(var1, var2, var3, var4 + 1);
			}

			if(material == Material::s_lava) {
				var10 |= flow(var1, var2 - 1, var3, var4);
				var10 |= flow(var1, var2 + 1, var3, var4);
				var10 |= flow(var1, var2, var3, var4 - 1);
				var10 |= flow(var1, var2, var3, var4 + 1);
			}

			if(!var10) {
				var1->setTileNoUpdate(var2, var3, var4, m_stillId1);
			} else {
				var1->scheduleBlockUpdate(var2, var3, var4, m_movingId1);
			}

			return var10;
		}
}

bool BlockFlowing::liquidSpread(World* var1, int var2, int var3, int var4, int var5, int var6, int var7) {
		if(canFlow(var1, var5, var6, var7)) {
			var1->setBlockWithNotify(var5, var6, var7, blockID);
			var1->scheduleBlockUpdate(var5, var6, var7, blockID);
			return true;
		} else {
			return false;
		}
}

bool BlockFlowing::liquidSpread2(World* var1, int var2, int var3, int var4, int var5, int var6, int var7) {
		if(!canFlow(var1, var5, var6, var7)) {
			return false;
		} else {
			var2 = var1->fluidFlowCheck(var2, var3, var4, m_movingId1, m_stillId1);
			if(var2 != -9999) {
				if(var2 < 0) {
					return false;
				}

				var3 = var2 % 1024;
				var2 >>= 10;
				var4 = var2 % 1024;
				var2 >>= 10;
				var2 %= 1024;
				if((var2 > var6 || !canFlow(var1, var5, var6 - 1, var7)) && var2 <= var6 && var3 != 0 && var3 != var1->width - 1 && var4 != 0 && var4 != 4 /* PORT: quad vertex count */ - 1) {
					return false;
				}

				var1->setBlockWithNotify(var3, var2, var4, 0);
			}

			var1->setBlockWithNotify(var5, var6, var7, blockID);
			var1->scheduleBlockUpdate(var5, var6, var7, blockID);
			return true;
		}
}

bool BlockFlowing::shouldSideBeRendered(World* var1, int var2, int var3, int var4, int var5) {
		if(var2 >= 0 && var3 >= 0 && var4 >= 0 && var2 < var1->width && var4 < var1_count /* PORT: set this from allocation */) {
			int var6 = var1->getBlockId(var2, var3, var4);
			return var6 != m_movingId1 && var6 != m_stillId1 ? (var5 != 1 || var1->getBlockId(var2 - 1, var3, var4) != 0 && var1->getBlockId(var2 + 1, var3, var4) != 0 && var1->getBlockId(var2, var3, var4 - 1) != 0 && var1->getBlockId(var2, var3, var4 + 1) != 0 ? BlockFluid::shouldSideBeRendered(var1, var2, var3, var4, var5) : true) : false;
		} else {
			return false;
		}
}

bool BlockFlowing::isCollidable() {
		return false;
}

AxisAlignedBB BlockFlowing::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return AxisAlignedBB();
}

bool BlockFlowing::isOpaqueCube() {
		return false;
}

void BlockFlowing::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
	// TODO: implement
}

int BlockFlowing::tickRate() {
		return material == Material::s_lava ? 25 : 5;
}

int BlockFlowing::quantityDropped(Random* var1) {
		return 0;
}

int BlockFlowing::getRenderBlockPass() {
		return material == Material::s_water ? 1 : 0;
}

bool BlockFlowing::extinguishFireLava(World* var0, int var1, int var2, int var3) {
		if(var0->getBlockId(var1, var2, var3) == Block::s_fire->blockID) {
			var0->setBlockWithNotify(var1, var2, var3, 0);
			return true;
		} else if(var0->getBlockId(var1, var2, var3) != Block::s_lavaMoving->blockID && var0->getBlockId(var1, var2, var3) != Block::s_lavaStill->blockID) {
			return false;
		} else {
			var0->setBlockWithNotify(var1, var2, var3, Block::s_stone->blockID);
			return true;
		}
}

bool BlockFlowing::flow(World* var0, int var1, int var2, int var3) {
		if(Block::s_fire->getChanceOfNeighborsEncouragingFire(var0->getBlockId(var1, var2, var3))) {
			Block::s_fire->fireSpread(var0, var1, var2, var3);
			return true;
		} else {
			return false;
		}
}
