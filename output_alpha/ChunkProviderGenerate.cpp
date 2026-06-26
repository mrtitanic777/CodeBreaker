#include "stdafx.h"
// Auto-generated from ChunkProviderGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - biome buffer is MobSpawnerBase** (BiomeGenBase[]); var4[i] is a single MobSpawnerBase*.
//  - MobSpawnerBase.<biome> static refs -> MobSpawnerBase::s_<biome>.
//  - biome.topBlock/fillerBlock -> biome->m_topBlock/m_fillerBlock.
//  - WorldChunkManager.temperature/humidity -> ->m_temperature/->m_humidity (+ include WCM).
//  - BlockSand.fallInstantly -> BlockSand::s_fallInstantly.
//  - included BlockFlower.h / BlockSand.h (their statics are dereferenced here).

#include "ChunkProviderGenerate.h"
#include "Common.h"
#include "Chunk.h"
#include "IProgressUpdate.h"
#include "MapGenBase.h"
#include "MobSpawnerBase.h"
#include "NoiseGeneratorOctaves.h"
#include "Random.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "Block.h"
#include "BlockFlower.h"
#include "BlockSand.h"
#include "IChunkProvider.h"
#include "MapGenCaves.h"
#include "Material.h"
#include "WorldGenBigTree.h"
#include "WorldGenCactus.h"
#include "WorldGenClay.h"
#include "WorldGenDungeons.h"
#include "WorldGenFlowers.h"
#include "WorldGenLakes.h"
#include "WorldGenLiquids.h"
#include "WorldGenMinable.h"
#include "WorldGenPumpkin.h"
#include "WorldGenReed.h"
#include "WorldGenTrees.h"
#include "WorldGenerator.h"

ChunkProviderGenerate::ChunkProviderGenerate()
	: rand(NULL)
	, m_field_912_k(NULL)
	, m_field_911_l(NULL)
	, m_field_910_m(NULL)
	, m_field_909_n(NULL)
	, m_field_908_o(NULL)
	, m_field_922_a(NULL)
	, m_field_921_b(NULL)
	, m_field_920_c(NULL)
	, worldObj(NULL)
	, m_field_4180_q(0)
	, m_field_905_r(new double[256])
	, m_field_904_s(new double[256])
	, m_field_903_t(new double[256])
	, m_field_902_u(new MapGenCaves())
	, m_biomesForGeneration(NULL)
	, m_field_4185_d(0)
	, m_field_4184_e(0)
	, m_field_4183_f(0)
	, m_field_4182_g(0)
	, m_field_4181_h(0)
	, m_field_914_i(NULL /* PORT: allocate int[32][32] */)
	, m_field_4178_w(0)
{
}

ChunkProviderGenerate::ChunkProviderGenerate(World* var1, long long var2)
	: rand(NULL)
	, m_field_912_k(NULL)
	, m_field_911_l(NULL)
	, m_field_910_m(NULL)
	, m_field_909_n(NULL)
	, m_field_908_o(NULL)
	, m_field_922_a(NULL)
	, m_field_921_b(NULL)
	, m_field_920_c(NULL)
	, worldObj(NULL)
	, m_field_4180_q(0)
	, m_field_905_r(new double[256])
	, m_field_904_s(new double[256])
	, m_field_903_t(new double[256])
	, m_field_902_u(new MapGenCaves())
	, m_biomesForGeneration(NULL)
	, m_field_4185_d(0)
	, m_field_4184_e(0)
	, m_field_4183_f(0)
	, m_field_4182_g(0)
	, m_field_4181_h(0)
	, m_field_914_i(NULL /* PORT: allocate int[32][32] */)
	, m_field_4178_w(0)
{
		worldObj = var1;
		rand = new Random(var2);
		m_field_912_k = new NoiseGeneratorOctaves(rand, 16);
		m_field_911_l = new NoiseGeneratorOctaves(rand, 16);
		m_field_910_m = new NoiseGeneratorOctaves(rand, 8);
		m_field_909_n = new NoiseGeneratorOctaves(rand, 4);
		m_field_908_o = new NoiseGeneratorOctaves(rand, 4);
		m_field_922_a = new NoiseGeneratorOctaves(rand, 10);
		m_field_921_b = new NoiseGeneratorOctaves(rand, 16);
		m_field_920_c = new NoiseGeneratorOctaves(rand, 8);
}

ChunkProviderGenerate::~ChunkProviderGenerate() {
	rand = NULL;
	m_field_912_k = NULL;
	m_field_911_l = NULL;
	m_field_910_m = NULL;
	m_field_909_n = NULL;
	m_field_908_o = NULL;
	m_field_922_a = NULL;
	m_field_921_b = NULL;
	m_field_920_c = NULL;
	worldObj = NULL;
	delete[] m_field_4180_q;
	m_field_4180_q = NULL;
	delete[] m_field_905_r;
	m_field_905_r = NULL;
	delete[] m_field_904_s;
	m_field_904_s = NULL;
	delete[] m_field_903_t;
	m_field_903_t = NULL;
	m_field_902_u = NULL;
	delete[] m_biomesForGeneration;
	m_biomesForGeneration = NULL;
	delete[] m_field_4185_d;
	m_field_4185_d = NULL;
	delete[] m_field_4184_e;
	m_field_4184_e = NULL;
	delete[] m_field_4183_f;
	m_field_4183_f = NULL;
	delete[] m_field_4182_g;
	m_field_4182_g = NULL;
	delete[] m_field_4181_h;
	m_field_4181_h = NULL;
	delete[] m_field_914_i;
	m_field_914_i = NULL;
	delete[] m_field_4178_w;
	m_field_4178_w = NULL;
}

void ChunkProviderGenerate::generateTerrain(int var1, int var2, unsigned char* var3, MobSpawnerBase** var4, double* var5) {
		unsigned char var6 = 4;
		unsigned char var7 = 64;
		int var8 = var6 + 1;
		unsigned char var9 = 17;
		int var10 = var6 + 1;
		m_field_4180_q = func_4061_a(m_field_4180_q, var1 * var6, 0, var2 * var6, var8, var9, var10);

		for(int var11 = 0; var11 < var6; ++var11) {
			for(int var12 = 0; var12 < var6; ++var12) {
				for(int var13 = 0; var13 < 16; ++var13) {
					double var14 = 0.125f;
					double var16 = m_field_4180_q[((var11 + 0) * var10 + var12 + 0) * var9 + var13 + 0];
					double var18 = m_field_4180_q[((var11 + 0) * var10 + var12 + 1) * var9 + var13 + 0];
					double var20 = m_field_4180_q[((var11 + 1) * var10 + var12 + 0) * var9 + var13 + 0];
					double var22 = m_field_4180_q[((var11 + 1) * var10 + var12 + 1) * var9 + var13 + 0];
					double var24 = (m_field_4180_q[((var11 + 0) * var10 + var12 + 0) * var9 + var13 + 1] - var16) * var14;
					double var26 = (m_field_4180_q[((var11 + 0) * var10 + var12 + 1) * var9 + var13 + 1] - var18) * var14;
					double var28 = (m_field_4180_q[((var11 + 1) * var10 + var12 + 0) * var9 + var13 + 1] - var20) * var14;
					double var30 = (m_field_4180_q[((var11 + 1) * var10 + var12 + 1) * var9 + var13 + 1] - var22) * var14;

					for(int var32 = 0; var32 < 8; ++var32) {
						double var33 = 0.25f;
						double var35 = var16;
						double var37 = var18;
						double var39 = (var20 - var16) * var33;
						double var41 = (var22 - var18) * var33;

						for(int var43 = 0; var43 < 4; ++var43) {
							int var44 = (var43 + var11 * 4) << 11 | (0 + var12 * 4) << 7 | var13 * 8 + var32;
							short var45 = 128;
							double var46 = 0.25f;
							double var48 = var35;
							double var50 = (var37 - var35) * var46;

							for(int var52 = 0; var52 < 4; ++var52) {
								double var53 = var5[(var11 * 4 + var43) * 16 + var12 * 4 + var52];
								int var55 = 0;
								if(var13 * 8 + var32 < var7) {
									if(var53 < 0.5f && var13 * 8 + var32 >= var7 - 1) {
										var55 = Block::s_blockIce->blockID;
									} else {
										var55 = Block::s_waterMoving->blockID;
									}
								}

								if(var48 > 0.0f) {
									var55 = Block::s_stone->blockID;
								}

								var3[var44] = (unsigned char)var55;
								var44 += var45;
								var48 += var50;
							}

							var35 += var39;
							var37 += var41;
						}

						var16 += var24;
						var18 += var26;
						var20 += var28;
						var22 += var30;
					}
				}
			}
		}

}

void ChunkProviderGenerate::replaceBlocksForBiome(int var1, int var2, unsigned char* var3, MobSpawnerBase** var4) {
		unsigned char var5 = 64;
		double var6 = 1.0f / 32.0f;
		m_field_905_r = m_field_909_n->func_807_a(m_field_905_r, (double)(var1 * 16), (double)(var2 * 16), 0.0f, 16, 16, 1, var6, var6, 1.0f);
		m_field_904_s = m_field_909_n->func_807_a(m_field_904_s, (double)(var2 * 16), 109.0134f, (double)(var1 * 16), 16, 1, 16, var6, 1.0f, var6);
		m_field_903_t = m_field_908_o->func_807_a(m_field_903_t, (double)(var1 * 16), (double)(var2 * 16), 0.0f, 16, 16, 1, var6 * 2.0f, var6 * 2.0f, var6 * 2.0f);

		for(int var8 = 0; var8 < 16; ++var8) {
			for(int var9 = 0; var9 < 16; ++var9) {
				MobSpawnerBase* var10 = var4[var8 * 16 + var9];
				bool var11 = m_field_905_r[var8 + var9 * 16] + rand->nextDouble() * 0.2f > 0.0f;
				bool var12 = m_field_904_s[var8 + var9 * 16] + rand->nextDouble() * 0.2f > 3.0f;
				int var13 = (int)(m_field_903_t[var8 + var9 * 16] / 3.0f + 3.0f + rand->nextDouble() * 0.25f);
				int var14 = -1;
				unsigned char var15 = var10->m_topBlock;
				unsigned char var16 = var10->m_fillerBlock;

				for(int var17 = 127; var17 >= 0; --var17) {
					int var18 = (var8 * 16 + var9) * 128 + var17;
					if(var17 <= 0 + rand->nextInt(5)) {
						var3[var18] = (unsigned char)Block::s_bedrock->blockID;
					} else {
						unsigned char var19 = var3[var18];
						if(var19 == 0) {
							var14 = -1;
						} else if(var19 == Block::s_stone->blockID) {
							if(var14 == -1) {
								if(var13 <= 0) {
									var15 = 0;
									var16 = (unsigned char)Block::s_stone->blockID;
								} else if(var17 >= var5 - 4 && var17 <= var5 + 1) {
									var15 = var10->m_topBlock;
									var16 = var10->m_fillerBlock;
									if(var12) {
										var15 = 0;
									}

									if(var12) {
										var16 = (unsigned char)Block::s_gravel->blockID;
									}

									if(var11) {
										var15 = (unsigned char)Block::s_sand->blockID;
									}

									if(var11) {
										var16 = (unsigned char)Block::s_sand->blockID;
									}
								}

								if(var17 < var5 && var15 == 0) {
									var15 = (unsigned char)Block::s_waterMoving->blockID;
								}

								var14 = var13;
								if(var17 >= var5 - 1) {
									var3[var18] = var15;
								} else {
									var3[var18] = var16;
								}
							} else if(var14 > 0) {
								--var14;
								var3[var18] = var16;
							}
						}
					}
				}
			}
		}

}

Chunk* ChunkProviderGenerate::provideChunk(int var1, int var2) {
		rand->setSeed((long)var1 * 341873128712L + (long)var2 * 132897987541L);
		unsigned char* var3 = new unsigned char[-(-32768)];
		Chunk* var4 = new Chunk(worldObj, var3, var1, var2);
		m_biomesForGeneration = worldObj->func_4075_a()->loadBlockGeneratorData(m_biomesForGeneration, var1 * 16, var2 * 16, 16, 16);
		double* var5 = worldObj->func_4075_a()->m_temperature;
		generateTerrain(var1, var2, var3, m_biomesForGeneration, var5);
		replaceBlocksForBiome(var1, var2, var3, m_biomesForGeneration);
		m_field_902_u->func_867_a(this, worldObj, var1, var2, var3);
		var4->func_1024_c();
		return var4;
}

double* ChunkProviderGenerate::func_4061_a(double* var1, int var2, int var3, int var4, int var5, int var6, int var7) {
		if(var1 == NULL) {
			var1 = new double[var5 * var6 * var7];
		}

		double var8 = 684.412f;
		double var10 = 684.412f;
		double* var12 = worldObj->func_4075_a()->m_temperature;
		double* var13 = worldObj->func_4075_a()->m_humidity;
		m_field_4182_g = m_field_922_a->func_4109_a(m_field_4182_g, var2, var4, var5, var7, 1.121f, 1.121f, 0.5f);
		m_field_4181_h = m_field_921_b->func_4109_a(m_field_4181_h, var2, var4, var5, var7, 200.0f, 200.0f, 0.5f);
		m_field_4185_d = m_field_910_m->func_807_a(m_field_4185_d, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8 / 80.0f, var10 / 160.0f, var8 / 80.0f);
		m_field_4184_e = m_field_912_k->func_807_a(m_field_4184_e, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
		m_field_4183_f = m_field_911_l->func_807_a(m_field_4183_f, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
		int var14 = 0;
		int var15 = 0;
		int var16 = 16 / var5;

		for(int var17 = 0; var17 < var5; ++var17) {
			int var18 = var17 * var16 + var16 / 2;

			for(int var19 = 0; var19 < var7; ++var19) {
				int var20 = var19 * var16 + var16 / 2;
				double var21 = var12[var18 * 16 + var20];
				double var23 = var13[var18 * 16 + var20] * var21;
				double var25 = 1.0f - var23;
				var25 *= var25;
				var25 *= var25;
				var25 = 1.0f - var25;
				double var27 = (m_field_4182_g[var15] + 256.0f) / 512.0f;
				var27 *= var25;
				if(var27 > 1.0f) {
					var27 = 1.0f;
				}

				double var29 = m_field_4181_h[var15] / 8000.0f;
				if(var29 < 0.0f) {
					var29 = -var29 * 0.3f;
				}

				var29 = var29 * 3.0f - 2.0f;
				if(var29 < 0.0f) {
					var29 /= 2.0f;
					if(var29 < -1.0f) {
						var29 = -1.0f;
					}

					var29 /= 1.4f;
					var29 /= 2.0f;
					var27 = 0.0f;
				} else {
					if(var29 > 1.0f) {
						var29 = 1.0f;
					}

					var29 /= 8.0f;
				}

				if(var27 < 0.0f) {
					var27 = 0.0f;
				}

				var27 += 0.5f;
				var29 = var29 * (double)var6 / 16.0f;
				double var31 = (double)var6 / 2.0f + var29 * 4.0f;
				++var15;

				for(int var33 = 0; var33 < var6; ++var33) {
					double var34 = 0.0f;
					double var36 = ((double)var33 - var31) * 12.0f / var27;
					if(var36 < 0.0f) {
						var36 *= 4.0f;
					}

					double var38 = m_field_4184_e[var14] / 512.0f;
					double var40 = m_field_4183_f[var14] / 512.0f;
					double var42 = (m_field_4185_d[var14] / 10.0f + 1.0f) / 2.0f;
					if(var42 < 0.0f) {
						var34 = var38;
					} else if(var42 > 1.0f) {
						var34 = var40;
					} else {
						var34 = var38 + (var40 - var38) * var42;
					}

					var34 -= var36;
					if(var33 > var6 - 4) {
						double var44 = (double)((float)(var33 - (var6 - 4)) / 3.0F);
						var34 = var34 * (1.0f - var44) + -10.0f * var44;
					}

					var1[var14] = var34;
					++var14;
				}
			}
		}

		return var1;
}

bool ChunkProviderGenerate::chunkExists(int var1, int var2) {
		return true;
}

void ChunkProviderGenerate::populate(IChunkProvider* var1, int var2, int var3) {
		BlockSand::s_fallInstantly = true;
		int var4 = var2 * 16;
		int var5 = var3 * 16;
		MobSpawnerBase* var6 = worldObj->func_4075_a()->func_4073_a(var4 + 16, var5 + 16);
		rand->setSeed(worldObj->m_randomSeed);
		long var7 = rand->nextLong() / 2L * 2L + 1L;
		long var9 = rand->nextLong() / 2L * 2L + 1L;
		rand->setSeed((long)var2 * var7 + (long)var3 * var9 ^ worldObj->m_randomSeed);
		double var11 = 0.25f;
		int var13;
		int var14;
		int var15;
		if(rand->nextInt(4) == 0) {
			var13 = var4 + rand->nextInt(16) + 8;
			var14 = rand->nextInt(128);
			var15 = var5 + rand->nextInt(16) + 8;
			(new WorldGenLakes(Block::s_waterMoving->blockID))->generate(worldObj, rand, var13, var14, var15);
		}

		if(rand->nextInt(8) == 0) {
			var13 = var4 + rand->nextInt(16) + 8;
			var14 = rand->nextInt(rand->nextInt(120) + 8);
			var15 = var5 + rand->nextInt(16) + 8;
			if(var14 < 64 || rand->nextInt(10) == 0) {
				(new WorldGenLakes(Block::s_lavaMoving->blockID))->generate(worldObj, rand, var13, var14, var15);
			}
		}

		int var16;
		for(var13 = 0; var13 < 8; ++var13) {
			var14 = var4 + rand->nextInt(16) + 8;
			var15 = rand->nextInt(128);
			var16 = var5 + rand->nextInt(16) + 8;
			(new WorldGenDungeons())->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 10; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(128);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenClay(32))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 20; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(128);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_dirt->blockID, 32))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 10; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(128);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_gravel->blockID, 32))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 20; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(128);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_oreCoal->blockID, 16))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 20; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(64);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_oreIron->blockID, 8))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 2; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(32);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_oreGold->blockID, 8))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 8; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(16);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_oreRedstone->blockID, 7))->generate(worldObj, rand, var14, var15, var16);
		}

		for(var13 = 0; var13 < 1; ++var13) {
			var14 = var4 + rand->nextInt(16);
			var15 = rand->nextInt(16);
			var16 = var5 + rand->nextInt(16);
			(new WorldGenMinable(Block::s_oreDiamond->blockID, 7))->generate(worldObj, rand, var14, var15, var16);
		}

		var11 = 0.5f;
		var13 = (int)((m_field_920_c->func_806_a((double)var4 * var11, (double)var5 * var11) / 8.0f + rand->nextDouble() * 4.0f + 4.0f) / 3.0f);
		var14 = 0;
		if(rand->nextInt(10) == 0) {
			++var14;
		}

		if(var6 == MobSpawnerBase::s_forest) {
			var14 += var13 + 5;
		}

		if(var6 == MobSpawnerBase::s_rainforest) {
			var14 += var13 + 5;
		}

		if(var6 == MobSpawnerBase::s_seasonalForest) {
			var14 += var13 + 2;
		}

		if(var6 == MobSpawnerBase::s_taiga) {
			var14 += var13 + 5;
		}

		if(var6 == MobSpawnerBase::s_desert) {
			var14 -= 20;
		}

		if(var6 == MobSpawnerBase::s_tundra) {
			var14 -= 20;
		}

		if(var6 == MobSpawnerBase::s_plains) {
			var14 -= 20;
		}

		void* var24 = new WorldGenTrees();
		if(rand->nextInt(10) == 0) {
			var24 = new WorldGenBigTree();
		}

		if(var6 == MobSpawnerBase::s_rainforest && rand->nextInt(3) == 0) {
			var24 = new WorldGenBigTree();
		}

		int var17;
		int var18;
		for(var16 = 0; var16 < var14; ++var16) {
			var17 = var4 + rand->nextInt(16) + 8;
			var18 = var5 + rand->nextInt(16) + 8;
			((WorldGenerator*)var24)->func_517_a(1.0f, 1.0f, 1.0f);
			((WorldGenerator*)var24)->generate(worldObj, rand, var17, worldObj->getHeightValue(var17, var18), var18);
		}

		int var19;
		for(var16 = 0; var16 < 2; ++var16) {
			var17 = var4 + rand->nextInt(16) + 8;
			var18 = rand->nextInt(128);
			var19 = var5 + rand->nextInt(16) + 8;
			(new WorldGenFlowers(Block::s_plantYellow->blockID))->generate(worldObj, rand, var17, var18, var19);
		}

		if(rand->nextInt(2) == 0) {
			var16 = var4 + rand->nextInt(16) + 8;
			var17 = rand->nextInt(128);
			var18 = var5 + rand->nextInt(16) + 8;
			(new WorldGenFlowers(Block::s_plantRed->blockID))->generate(worldObj, rand, var16, var17, var18);
		}

		if(rand->nextInt(4) == 0) {
			var16 = var4 + rand->nextInt(16) + 8;
			var17 = rand->nextInt(128);
			var18 = var5 + rand->nextInt(16) + 8;
			(new WorldGenFlowers(Block::s_mushroomBrown->blockID))->generate(worldObj, rand, var16, var17, var18);
		}

		if(rand->nextInt(8) == 0) {
			var16 = var4 + rand->nextInt(16) + 8;
			var17 = rand->nextInt(128);
			var18 = var5 + rand->nextInt(16) + 8;
			(new WorldGenFlowers(Block::s_mushroomRed->blockID))->generate(worldObj, rand, var16, var17, var18);
		}

		for(var16 = 0; var16 < 10; ++var16) {
			var17 = var4 + rand->nextInt(16) + 8;
			var18 = rand->nextInt(128);
			var19 = var5 + rand->nextInt(16) + 8;
			(new WorldGenReed())->generate(worldObj, rand, var17, var18, var19);
		}

		if(rand->nextInt(32) == 0) {
			var16 = var4 + rand->nextInt(16) + 8;
			var17 = rand->nextInt(128);
			var18 = var5 + rand->nextInt(16) + 8;
			(new WorldGenPumpkin())->generate(worldObj, rand, var16, var17, var18);
		}

		var16 = 0;
		if(var6 == MobSpawnerBase::s_desert) {
			var16 += 10;
		}

		int var20;
		for(var17 = 0; var17 < var16; ++var17) {
			var18 = var4 + rand->nextInt(16) + 8;
			var19 = rand->nextInt(128);
			var20 = var5 + rand->nextInt(16) + 8;
			(new WorldGenCactus())->generate(worldObj, rand, var18, var19, var20);
		}

		for(var17 = 0; var17 < 50; ++var17) {
			var18 = var4 + rand->nextInt(16) + 8;
			var19 = rand->nextInt(rand->nextInt(120) + 8);
			var20 = var5 + rand->nextInt(16) + 8;
			(new WorldGenLiquids(Block::s_waterStill->blockID))->generate(worldObj, rand, var18, var19, var20);
		}

		for(var17 = 0; var17 < 20; ++var17) {
			var18 = var4 + rand->nextInt(16) + 8;
			var19 = rand->nextInt(rand->nextInt(rand->nextInt(112) + 8) + 8);
			var20 = var5 + rand->nextInt(16) + 8;
			(new WorldGenLiquids(Block::s_lavaStill->blockID))->generate(worldObj, rand, var18, var19, var20);
		}

		m_field_4178_w = worldObj->func_4075_a()->getTemperatures(m_field_4178_w, var4 + 8, var5 + 8, 16, 16);

		for(var17 = var4 + 8; var17 < var4 + 8 + 16; ++var17) {
			for(var18 = var5 + 8; var18 < var5 + 8 + 16; ++var18) {
				var19 = var17 - (var4 + 8);
				var20 = var18 - (var5 + 8);
				int var21 = worldObj->func_4083_e(var17, var18);
				double var22 = m_field_4178_w[var19 * 16 + var20] - (double)(var21 - 64) / 64.0f * 0.3f;
				if(var22 < 0.5f && var21 > 0 && var21 < 128 && worldObj->getBlockId(var17, var21, var18) == 0 && worldObj->getBlockMaterial(var17, var21 - 1, var18)->func_880_c() && worldObj->getBlockMaterial(var17, var21 - 1, var18) != Material::s_ice) {
					worldObj->setBlockWithNotify(var17, var21, var18, Block::s_snow->blockID);
				}
			}
		}

		BlockSand::s_fallInstantly = false;
}

bool ChunkProviderGenerate::saveChunks(bool var1, IProgressUpdate* var2) {
		return true;
}

bool ChunkProviderGenerate::func_532_a() {
		return false;
}

bool ChunkProviderGenerate::func_536_b() {
		return true;
}
