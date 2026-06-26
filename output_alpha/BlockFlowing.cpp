#include "stdafx.h"
// Auto-generated from BlockFlowing.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlowing.h"
#include "Common.h"
#include "Material.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFluids.h"
#include "WorldProvider.h"

BlockFlowing::BlockFlowing()
	: m_field_460_a(0)
	, m_field_459_b(new bool[4])
	, m_field_461_c(new int[4])
{
}

BlockFlowing::BlockFlowing(int var1, Material* var2)
	: BlockFluids(var1, var2)
	, m_field_460_a(0)
	, m_field_459_b(new bool[4])
	, m_field_461_c(new int[4])
{
}

BlockFlowing::~BlockFlowing() {
	delete[] m_field_459_b;
	m_field_459_b = NULL;
	delete[] m_field_461_c;
	m_field_461_c = NULL;
}

void BlockFlowing::func_15233_j(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		var1->setBlockAndMetadata(var2, var3, var4, blockID + 1, var5);
		var1->func_701_b(var2, var3, var4, var2, var3, var4);
		var1->func_665_h(var2, var3, var4);
}

void BlockFlowing::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		int var6 = func_290_h(var1, var2, var3, var4);
		unsigned char var7 = 1;
		if(m_blockMaterial == Material::s_lava && !var1->m_worldProvider->m_field_6479_d) {
			var7 = 2;
		}

		bool var8 = true;
		int var10;
		if(var6 > 0) {
			unsigned char var9 = -100;
			m_field_460_a = 0;
			int var12 = func_296_f(var1, var2 - 1, var3, var4, var9);
			var12 = func_296_f(var1, var2 + 1, var3, var4, var12);
			var12 = func_296_f(var1, var2, var3, var4 - 1, var12);
			var12 = func_296_f(var1, var2, var3, var4 + 1, var12);
			var10 = var12 + var7;
			if(var10 >= 8 || var12 < 0) {
				var10 = -1;
			}

			if(func_290_h(var1, var2, var3 + 1, var4) >= 0) {
				int var11 = func_290_h(var1, var2, var3 + 1, var4);
				if(var11 >= 8) {
					var10 = var11;
				} else {
					var10 = var11 + 8;
				}
			}

			if(m_field_460_a >= 2 && m_blockMaterial == Material::s_water) {
				if(var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
					var10 = 0;
				} else if(var1->getBlockMaterial(var2, var3 - 1, var4) == m_blockMaterial && var1->getBlockMetadata(var2, var3, var4) == 0) {
					var10 = 0;
				}
			}

			if(m_blockMaterial == Material::s_lava && var6 < 8 && var10 < 8 && var10 > var6 && var5->nextInt(4) != 0) {
				var10 = var6;
				var8 = false;
			}

			if(var10 != var6) {
				var6 = var10;
				if(var10 < 0) {
					var1->setBlockWithNotify(var2, var3, var4, 0);
				} else {
					var1->setBlockMetadataWithNotify(var2, var3, var4, var10);
					var1->scheduleBlockUpdate(var2, var3, var4, blockID);
					var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
				}
			} else if(var8) {
				func_15233_j(var1, var2, var3, var4);
			}
		} else {
			func_15233_j(var1, var2, var3, var4);
		}

		if(func_298_m(var1, var2, var3 - 1, var4)) {
			if(var6 >= 8) {
				var1->setBlockAndMetadataWithNotify(var2, var3 - 1, var4, blockID, var6);
			} else {
				var1->setBlockAndMetadataWithNotify(var2, var3 - 1, var4, blockID, var6 + 8);
			}
		} else if(var6 >= 0 && (var6 == 0 || func_295_l(var1, var2, var3 - 1, var4))) {
			bool* var13 = func_297_k(var1, var2, var3, var4);
			var10 = var6 + var7;
			if(var6 >= 8) {
				var10 = 1;
			}

			if(var10 >= 8) {
				return;
			}

			if(var13[0]) {
				func_299_g(var1, var2 - 1, var3, var4, var10);
			}

			if(var13[1]) {
				func_299_g(var1, var2 + 1, var3, var4, var10);
			}

			if(var13[2]) {
				func_299_g(var1, var2, var3, var4 - 1, var10);
			}

			if(var13[3]) {
				func_299_g(var1, var2, var3, var4 + 1, var10);
			}
		}

}

void BlockFlowing::func_299_g(World* var1, int var2, int var3, int var4, int var5) {
		if(func_298_m(var1, var2, var3, var4)) {
			int var6 = var1->getBlockId(var2, var3, var4);
			if(var6 > 0) {
				if(m_blockMaterial == Material::s_lava) {
					func_292_i(var1, var2, var3, var4);
				} else {
					Block::blocksList[var6]->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
				}
			}

			var1->setBlockAndMetadataWithNotify(var2, var3, var4, blockID, var5);
		}

}

int BlockFlowing::func_300_a(World* var1, int var2, int var3, int var4, int var5, int var6) {
		int var7 = 1000;

		for(int var8 = 0; var8 < 4; ++var8) {
			if((var8 != 0 || var6 != 1) && (var8 != 1 || var6 != 0) && (var8 != 2 || var6 != 3) && (var8 != 3 || var6 != 2)) {
				int var9 = var2;
				int var11 = var4;
				if(var8 == 0) {
					var9 = var2 - 1;
				}

				if(var8 == 1) {
					++var9;
				}

				if(var8 == 2) {
					var11 = var4 - 1;
				}

				if(var8 == 3) {
					++var11;
				}

				if(!func_295_l(var1, var9, var3, var11) && (var1->getBlockMaterial(var9, var3, var11) != m_blockMaterial || var1->getBlockMetadata(var9, var3, var11) != 0)) {
					if(!func_295_l(var1, var9, var3 - 1, var11)) {
						return var5;
					}

					if(var5 < 4) {
						int var12 = func_300_a(var1, var9, var3, var11, var5 + 1, var8);
						if(var12 < var7) {
							var7 = var12;
						}
					}
				}
			}
		}

		return var7;
}

bool* BlockFlowing::func_297_k(World* var1, int var2, int var3, int var4) {
		int var5;
		int var6;
		for(var5 = 0; var5 < 4; ++var5) {
			m_field_461_c[var5] = 1000;
			var6 = var2;
			int var8 = var4;
			if(var5 == 0) {
				var6 = var2 - 1;
			}

			if(var5 == 1) {
				++var6;
			}

			if(var5 == 2) {
				var8 = var4 - 1;
			}

			if(var5 == 3) {
				++var8;
			}

			if(!func_295_l(var1, var6, var3, var8) && (var1->getBlockMaterial(var6, var3, var8) != m_blockMaterial || var1->getBlockMetadata(var6, var3, var8) != 0)) {
				if(!func_295_l(var1, var6, var3 - 1, var8)) {
					m_field_461_c[var5] = 0;
				} else {
					m_field_461_c[var5] = func_300_a(var1, var6, var3, var8, 1, var5);
				}
			}
		}

		var5 = m_field_461_c[0];

		for(var6 = 1; var6 < 4; ++var6) {
			if(m_field_461_c[var6] < var5) {
				var5 = m_field_461_c[var6];
			}
		}

		for(var6 = 0; var6 < 4; ++var6) {
			m_field_459_b[var6] = m_field_461_c[var6] == var5;
		}

		return m_field_459_b;
}

bool BlockFlowing::func_295_l(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockId(var2, var3, var4);
		if(var5 != Block::s_doorWood->blockID && var5 != Block::s_doorSteel->blockID && var5 != Block::s_signPost->blockID && var5 != Block::s_ladder->blockID && var5 != Block::s_reed->blockID) {
			if(var5 == 0) {
				return false;
			} else {
				Material* var6 = Block::blocksList[var5]->m_blockMaterial;
				return var6->func_878_a();
			}
		} else {
			return true;
		}
}

int BlockFlowing::func_296_f(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = func_290_h(var1, var2, var3, var4);
		if(var6 < 0) {
			return var5;
		} else {
			if(var6 == 0) {
				++m_field_460_a;
			}

			if(var6 >= 8) {
				var6 = 0;
			}

			return var5 >= 0 && var6 >= var5 ? var5 : var6;
		}
}

bool BlockFlowing::func_298_m(World* var1, int var2, int var3, int var4) {
		Material* var5 = var1->getBlockMaterial(var2, var3, var4);
		return var5 == m_blockMaterial ? false : (var5 == Material::s_lava ? false : !func_295_l(var1, var2, var3, var4));
}

void BlockFlowing::onBlockAdded(World* var1, int var2, int var3, int var4) {
		BlockFluids::onBlockAdded(var1, var2, var3, var4);
		if(var1->getBlockId(var2, var3, var4) == blockID) {
			var1->scheduleBlockUpdate(var2, var3, var4, blockID);
		}

}
