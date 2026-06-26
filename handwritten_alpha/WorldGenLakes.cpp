#include "stdafx.h"
// Auto-generated from WorldGenLakes.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: removed duplicate `var6_count` redefinitions (converter emitted 4).

#include "WorldGenLakes.h"
#include "Common.h"
#include "BlockGrass.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "EnumSkyBlock.h"

WorldGenLakes::WorldGenLakes()
	: m_field_15235_a(0)
{
}

WorldGenLakes::WorldGenLakes(int var1)
	: m_field_15235_a(0)
{
		m_field_15235_a = var1;
}

WorldGenLakes::~WorldGenLakes() {
	// No dynamic resources
}

bool WorldGenLakes::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		var3 -= 8;

		for(var5 -= 8; var4 > 0 && var1->getBlockId(var3, var4, var5) == 0; --var4) {
		}

		var4 -= 4;
		bool* var6 = new bool[2048];
		// PORT: zero-init the lake voxel mask (Java new boolean[] is zeroed; C++ new bool[] is not).
		for(int _z = 0; _z < 2048; ++_z) {
			var6[_z] = false;
		}
		int var7 = var2->nextInt(4) + 4;

		int var8;
		for(var8 = 0; var8 < var7; ++var8) {
			double var9 = var2->nextDouble() * 6.0f + 3.0f;
			double var11 = var2->nextDouble() * 4.0f + 2.0f;
			double var13 = var2->nextDouble() * 6.0f + 3.0f;
			double var15 = var2->nextDouble() * (16.0f - var9 - 2.0f) + 1.0f + var9 / 2.0f;
			double var17 = var2->nextDouble() * (8.0f - var11 - 4.0f) + 2.0f + var11 / 2.0f;
			double var19 = var2->nextDouble() * (16.0f - var13 - 2.0f) + 1.0f + var13 / 2.0f;

			for(int var21 = 1; var21 < 15; ++var21) {
				for(int var22 = 1; var22 < 15; ++var22) {
					for(int var23 = 1; var23 < 7; ++var23) {
						double var24 = ((double)var21 - var15) / (var9 / 2.0f);
						double var26 = ((double)var23 - var17) / (var11 / 2.0f);
						double var28 = ((double)var22 - var19) / (var13 / 2.0f);
						double var30 = var24 * var24 + var26 * var26 + var28 * var28;
						if(var30 < 1.0f) {
							var6[(var21 * 16 + var22) * 8 + var23] = true;
						}
					}
				}
			}
		}

		int var10;
		int var32;
		for(var8 = 0; var8 < 16; ++var8) {
			for(var32 = 0; var32 < 16; ++var32) {
				for(var10 = 0; var10 < 8; ++var10) {
					bool var33 = !var6[(var8 * 16 + var32) * 8 + var10] && (var8 < 15 && var6[((var8 + 1) * 16 + var32) * 8 + var10] || var8 > 0 && var6[((var8 - 1) * 16 + var32) * 8 + var10] || var32 < 15 && var6[(var8 * 16 + var32 + 1) * 8 + var10] || var32 > 0 && var6[(var8 * 16 + (var32 - 1)) * 8 + var10] || var10 < 7 && var6[(var8 * 16 + var32) * 8 + var10 + 1] || var10 > 0 && var6[(var8 * 16 + var32) * 8 + (var10 - 1)]);
					if(var33) {
						Material* var12 = var1->getBlockMaterial(var3 + var8, var4 + var10, var5 + var32);
						if(var10 >= 4 && var12->getIsLiquid()) {
							delete[] var6;
							return false;
						}

						if(var10 < 4 && !var12->func_878_a() && var1->getBlockId(var3 + var8, var4 + var10, var5 + var32) != m_field_15235_a) {
							delete[] var6;
							return false;
						}
					}
				}
			}
		}

		for(var8 = 0; var8 < 16; ++var8) {
			for(var32 = 0; var32 < 16; ++var32) {
				for(var10 = 0; var10 < 8; ++var10) {
					if(var6[(var8 * 16 + var32) * 8 + var10]) {
						var1->setBlockWithNotify(var3 + var8, var4 + var10, var5 + var32, var10 >= 4 ? 0 : m_field_15235_a);
					}
				}
			}
		}

		for(var8 = 0; var8 < 16; ++var8) {
			for(var32 = 0; var32 < 16; ++var32) {
				for(var10 = 4; var10 < 8; ++var10) {
					if(var6[(var8 * 16 + var32) * 8 + var10] && var1->getBlockId(var3 + var8, var4 + var10 - 1, var5 + var32) == Block::s_dirt->blockID && var1->getSavedLightValue(EnumSkyBlock::s_Sky, var3 + var8, var4 + var10, var5 + var32) > 0) {
						var1->setBlockWithNotify(var3 + var8, var4 + var10 - 1, var5 + var32, Block::s_grass->blockID);
					}
				}
			}
		}

		delete[] var6;
		return true;
}
