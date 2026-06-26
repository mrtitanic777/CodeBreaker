#include "stdafx.h"
// Hand-ported from ChunkProviderHell.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// The Nether terrain generator: octave-noise density field carved into a 128-tall
// netherrack ("bloodStone") slab over a lava sea, with a bedrock cap/floor, a
// soulsand/gravel surface pass, cave carving (MapGenCavesHell) and decoration
// populate (lava springs, fire, glowstone, mushrooms).
//
// PORT NOTES:
//  - BlockSand.fallInstantly is the static gate `BlockSand::s_fallInstantly`.
//  - Block::s_mushroomBrown / s_mushroomRed are declared as BlockFlower* in
//    Block.h, so BlockFlower.h must be fully included (not just forward-declared)
//    before dereferencing ->blockID / ->generate.
//  - func_4059_a's index expression keeps Java precedence; parentheses added
//    only to silence MSVC C4554 (no semantic change: Java binds + > << > |).

#include "ChunkProviderHell.h"
#include "Common.h"
#include "Chunk.h"
#include "IProgressUpdate.h"
#include "MapGenBase.h"
#include "NoiseGeneratorOctaves.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "BlockSand.h"
#include "BlockFlower.h"
#include "IChunkProvider.h"
#include "MapGenCavesHell.h"
#include "WorldGenFire.h"
#include "WorldGenFlowers.h"
#include "WorldGenHellLava.h"
#include "WorldGenLightStone1.h"
#include "WorldGenLightStone2.h"
#include <math.h>

ChunkProviderHell::ChunkProviderHell()
	: m_field_4177_a(NULL)
	, m_field_4176_b(NULL)
	, m_field_4170_h(NULL)
	, m_field_4169_i(NULL)
	, m_field_4168_j(NULL)
	, m_field_4167_k(NULL)
	, m_field_4166_l(NULL)
	, m_field_4165_m(NULL)
	, m_field_4164_n(NULL)
	, m_field_4163_o(NULL)
	, m_field_4162_p(new double[256])
	, m_field_4161_q(new double[256])
	, m_field_4160_r(new double[256])
	, m_field_4159_s(new MapGenCavesHell())
	, m_field_4175_c(NULL)
	, m_field_4174_d(NULL)
	, m_field_4173_e(NULL)
	, m_field_4172_f(NULL)
	, m_field_4171_g(NULL)
	, m_field_4163_o_count(0)
	, m_field_4162_p_count(256)
	, m_field_4161_q_count(256)
	, m_field_4160_r_count(256)
	, m_field_4175_c_count(0)
	, m_field_4174_d_count(0)
	, m_field_4173_e_count(0)
	, m_field_4172_f_count(0)
	, m_field_4171_g_count(0)
{
}

ChunkProviderHell::ChunkProviderHell(World* var1, long long var2)
	: m_field_4177_a(NULL)
	, m_field_4176_b(NULL)
	, m_field_4170_h(NULL)
	, m_field_4169_i(NULL)
	, m_field_4168_j(NULL)
	, m_field_4167_k(NULL)
	, m_field_4166_l(NULL)
	, m_field_4165_m(NULL)
	, m_field_4164_n(NULL)
	, m_field_4163_o(NULL)
	, m_field_4162_p(new double[256])
	, m_field_4161_q(new double[256])
	, m_field_4160_r(new double[256])
	, m_field_4159_s(new MapGenCavesHell())
	, m_field_4175_c(NULL)
	, m_field_4174_d(NULL)
	, m_field_4173_e(NULL)
	, m_field_4172_f(NULL)
	, m_field_4171_g(NULL)
	, m_field_4163_o_count(0)
	, m_field_4162_p_count(256)
	, m_field_4161_q_count(256)
	, m_field_4160_r_count(256)
	, m_field_4175_c_count(0)
	, m_field_4174_d_count(0)
	, m_field_4173_e_count(0)
	, m_field_4172_f_count(0)
	, m_field_4171_g_count(0)
{
	m_field_4164_n = var1;
	m_field_4170_h = new Random(var2);
	m_field_4169_i = new NoiseGeneratorOctaves(m_field_4170_h, 16);
	m_field_4168_j = new NoiseGeneratorOctaves(m_field_4170_h, 16);
	m_field_4167_k = new NoiseGeneratorOctaves(m_field_4170_h, 8);
	m_field_4166_l = new NoiseGeneratorOctaves(m_field_4170_h, 4);
	m_field_4165_m = new NoiseGeneratorOctaves(m_field_4170_h, 4);
	m_field_4177_a = new NoiseGeneratorOctaves(m_field_4170_h, 10);
	m_field_4176_b = new NoiseGeneratorOctaves(m_field_4170_h, 16);
}

ChunkProviderHell::~ChunkProviderHell() {
	m_field_4170_h = NULL;
	m_field_4169_i = NULL;
	m_field_4168_j = NULL;
	m_field_4167_k = NULL;
	m_field_4166_l = NULL;
	m_field_4165_m = NULL;
	m_field_4177_a = NULL;
	m_field_4176_b = NULL;
	m_field_4164_n = NULL;
	delete[] m_field_4163_o;
	m_field_4163_o = NULL;
	delete[] m_field_4162_p;
	m_field_4162_p = NULL;
	delete[] m_field_4161_q;
	m_field_4161_q = NULL;
	delete[] m_field_4160_r;
	m_field_4160_r = NULL;
	m_field_4159_s = NULL;
	delete[] m_field_4175_c;
	m_field_4175_c = NULL;
	delete[] m_field_4174_d;
	m_field_4174_d = NULL;
	delete[] m_field_4173_e;
	m_field_4173_e = NULL;
	delete[] m_field_4172_f;
	m_field_4172_f = NULL;
	delete[] m_field_4171_g;
	m_field_4171_g = NULL;
}

void ChunkProviderHell::func_4059_a(int var1, int var2, unsigned char* var3) {
	unsigned char var4 = 4;
	unsigned char var5 = 32;
	int var6 = var4 + 1;
	unsigned char var7 = 17;
	int var8 = var4 + 1;
	m_field_4163_o = func_4057_a(m_field_4163_o, var1 * var4, 0, var2 * var4, var6, var7, var8);

	for(int var9 = 0; var9 < var4; ++var9) {
		for(int var10 = 0; var10 < var4; ++var10) {
			for(int var11 = 0; var11 < 16; ++var11) {
				double var12 = 0.125f;
				double var14 = m_field_4163_o[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 0];
				double var16 = m_field_4163_o[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 0];
				double var18 = m_field_4163_o[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 0];
				double var20 = m_field_4163_o[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 0];
				double var22 = (m_field_4163_o[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 1] - var14) * var12;
				double var24 = (m_field_4163_o[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 1] - var16) * var12;
				double var26 = (m_field_4163_o[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 1] - var18) * var12;
				double var28 = (m_field_4163_o[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 1] - var20) * var12;

				for(int var30 = 0; var30 < 8; ++var30) {
					double var31 = 0.25f;
					double var33 = var14;
					double var35 = var16;
					double var37 = (var18 - var14) * var31;
					double var39 = (var20 - var16) * var31;

					for(int var41 = 0; var41 < 4; ++var41) {
						// Java precedence preserved: ((a)<<11) | ((b)<<7) | (c).
						int var42 = (((var41 + var9 * 4) << 11) | ((0 + var10 * 4) << 7)) | (var11 * 8 + var30);
						short var43 = 128;
						double var44 = 0.25f;
						double var46 = var33;
						double var48 = (var35 - var33) * var44;

						for(int var50 = 0; var50 < 4; ++var50) {
							int var51 = 0;
							if(var11 * 8 + var30 < var5) {
								var51 = Block::s_lavaMoving->blockID;
							}

							if(var46 > 0.0f) {
								var51 = Block::s_bloodStone->blockID;
							}

							var3[var42] = (unsigned char)var51;
							var42 += var43;
							var46 += var48;
						}

						var33 += var37;
						var35 += var39;
					}

					var14 += var22;
					var16 += var24;
					var18 += var26;
					var20 += var28;
				}
			}
		}
	}

}

void ChunkProviderHell::func_4058_b(int var1, int var2, unsigned char* var3) {
	unsigned char var4 = 64;
	double var5 = 1.0f / 32.0f;
	m_field_4162_p = m_field_4166_l->func_807_a(m_field_4162_p, (double)(var1 * 16), (double)(var2 * 16), 0.0f, 16, 16, 1, var5, var5, 1.0f);
	m_field_4161_q = m_field_4166_l->func_807_a(m_field_4161_q, (double)(var2 * 16), 109.0134f, (double)(var1 * 16), 16, 1, 16, var5, 1.0f, var5);
	m_field_4160_r = m_field_4165_m->func_807_a(m_field_4160_r, (double)(var1 * 16), (double)(var2 * 16), 0.0f, 16, 16, 1, var5 * 2.0f, var5 * 2.0f, var5 * 2.0f);

	for(int var7 = 0; var7 < 16; ++var7) {
		for(int var8 = 0; var8 < 16; ++var8) {
			bool var9 = m_field_4162_p[var7 + var8 * 16] + m_field_4170_h->nextDouble() * 0.2f > 0.0f;
			bool var10 = m_field_4161_q[var7 + var8 * 16] + m_field_4170_h->nextDouble() * 0.2f > 0.0f;
			int var11 = (int)(m_field_4160_r[var7 + var8 * 16] / 3.0f + 3.0f + m_field_4170_h->nextDouble() * 0.25f);
			int var12 = -1;
			unsigned char var13 = (unsigned char)Block::s_bloodStone->blockID;
			unsigned char var14 = (unsigned char)Block::s_bloodStone->blockID;

			for(int var15 = 127; var15 >= 0; --var15) {
				int var16 = (var7 * 16 + var8) * 128 + var15;
				if(var15 >= 127 - m_field_4170_h->nextInt(5)) {
					var3[var16] = (unsigned char)Block::s_bedrock->blockID;
				} else if(var15 <= 0 + m_field_4170_h->nextInt(5)) {
					var3[var16] = (unsigned char)Block::s_bedrock->blockID;
				} else {
					unsigned char var17 = var3[var16];
					if(var17 == 0) {
						var12 = -1;
					} else if(var17 == Block::s_bloodStone->blockID) {
						if(var12 == -1) {
							if(var11 <= 0) {
								var13 = 0;
								var14 = (unsigned char)Block::s_bloodStone->blockID;
							} else if(var15 >= var4 - 4 && var15 <= var4 + 1) {
								var13 = (unsigned char)Block::s_bloodStone->blockID;
								var14 = (unsigned char)Block::s_bloodStone->blockID;
								if(var10) {
									var13 = (unsigned char)Block::s_gravel->blockID;
								}

								if(var10) {
									var14 = (unsigned char)Block::s_bloodStone->blockID;
								}

								if(var9) {
									var13 = (unsigned char)Block::s_slowSand->blockID;
								}

								if(var9) {
									var14 = (unsigned char)Block::s_slowSand->blockID;
								}
							}

							if(var15 < var4 && var13 == 0) {
								var13 = (unsigned char)Block::s_lavaMoving->blockID;
							}

							var12 = var11;
							if(var15 >= var4 - 1) {
								var3[var16] = var13;
							} else {
								var3[var16] = var14;
							}
						} else if(var12 > 0) {
							--var12;
							var3[var16] = var14;
						}
					}
				}
			}
		}
	}

}

Chunk* ChunkProviderHell::provideChunk(int var1, int var2) {
	m_field_4170_h->setSeed((long)var1 * 341873128712L + (long)var2 * 132897987541L);
	unsigned char* var3 = new unsigned char[-(-32768)];
	func_4059_a(var1, var2, var3);
	func_4058_b(var1, var2, var3);
	m_field_4159_s->func_867_a(this, m_field_4164_n, var1, var2, var3);
	Chunk* var4 = new Chunk(m_field_4164_n, var3, var1, var2);
	var4->func_1024_c();
	var4->func_4143_d();
	return var4;
}

double* ChunkProviderHell::func_4057_a(double* var1, int var2, int var3, int var4, int var5, int var6, int var7) {
	if(var1 == NULL) {
		var1 = new double[var5 * var6 * var7];
	}

	double var8 = 684.412f;
	double var10 = 2053.236f;
	m_field_4172_f = m_field_4177_a->func_807_a(m_field_4172_f, (double)var2, (double)var3, (double)var4, var5, 1, var7, 1.0f, 0.0f, 1.0f);
	m_field_4171_g = m_field_4176_b->func_807_a(m_field_4171_g, (double)var2, (double)var3, (double)var4, var5, 1, var7, 100.0f, 0.0f, 100.0f);
	m_field_4175_c = m_field_4167_k->func_807_a(m_field_4175_c, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8 / 80.0f, var10 / 60.0f, var8 / 80.0f);
	m_field_4174_d = m_field_4169_i->func_807_a(m_field_4174_d, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
	m_field_4173_e = m_field_4168_j->func_807_a(m_field_4173_e, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
	int var12 = 0;
	int var13 = 0;
	double* var14 = new double[var6];
	int var14_count = var6;
	(void)var14_count;

	int var15;
	for(var15 = 0; var15 < var6; ++var15) {
		var14[var15] = cosf((float)((double)var15 * 3.14159265358979323846 * 6.0 / (double)var6)) * 2.0f;
		double var16 = (double)var15;
		if(var15 > var6 / 2) {
			var16 = (double)(var6 - 1 - var15);
		}

		if(var16 < 4.0f) {
			var16 = 4.0f - var16;
			var14[var15] -= var16 * var16 * var16 * 10.0f;
		}
	}

	for(var15 = 0; var15 < var5; ++var15) {
		for(int var36 = 0; var36 < var7; ++var36) {
			double var17 = (m_field_4172_f[var13] + 256.0f) / 512.0f;
			if(var17 > 1.0f) {
				var17 = 1.0f;
			}

			double var19 = 0.0f;
			double var21 = m_field_4171_g[var13] / 8000.0f;
			if(var21 < 0.0f) {
				var21 = -var21;
			}

			var21 = var21 * 3.0f - 3.0f;
			if(var21 < 0.0f) {
				var21 /= 2.0f;
				if(var21 < -1.0f) {
					var21 = -1.0f;
				}

				var21 /= 1.4f;
				var21 /= 2.0f;
				var17 = 0.0f;
			} else {
				if(var21 > 1.0f) {
					var21 = 1.0f;
				}

				var21 /= 6.0f;
			}

			var17 += 0.5f;
			var21 = var21 * (double)var6 / 16.0f;
			++var13;

			for(int var23 = 0; var23 < var6; ++var23) {
				double var24 = 0.0f;
				double var26 = var14[var23];
				double var28 = m_field_4174_d[var12] / 512.0f;
				double var30 = m_field_4173_e[var12] / 512.0f;
				double var32 = (m_field_4175_c[var12] / 10.0f + 1.0f) / 2.0f;
				if(var32 < 0.0f) {
					var24 = var28;
				} else if(var32 > 1.0f) {
					var24 = var30;
				} else {
					var24 = var28 + (var30 - var28) * var32;
				}

				var24 -= var26;
				double var34;
				if(var23 > var6 - 4) {
					var34 = (double)((float)(var23 - (var6 - 4)) / 3.0F);
					var24 = var24 * (1.0f - var34) + -10.0f * var34;
				}

				if((double)var23 < var19) {
					var34 = (var19 - (double)var23) / 4.0f;
					if(var34 < 0.0f) {
						var34 = 0.0f;
					}

					if(var34 > 1.0f) {
						var34 = 1.0f;
					}

					var24 = var24 * (1.0f - var34) + -10.0f * var34;
				}

				var1[var12] = var24;
				++var12;
			}
		}
	}

	delete[] var14;
	return var1;
}

bool ChunkProviderHell::chunkExists(int var1, int var2) {
	return true;
}

void ChunkProviderHell::populate(IChunkProvider* var1, int var2, int var3) {
	BlockSand::s_fallInstantly = true;
	int var4 = var2 * 16;
	int var5 = var3 * 16;

	int var6;
	int var7;
	int var8;
	int var9;
	for(var6 = 0; var6 < 8; ++var6) {
		var7 = var4 + m_field_4170_h->nextInt(16) + 8;
		var8 = m_field_4170_h->nextInt(120) + 4;
		var9 = var5 + m_field_4170_h->nextInt(16) + 8;
		(new WorldGenHellLava(Block::s_lavaStill->blockID))->generate(m_field_4164_n, m_field_4170_h, var7, var8, var9);
	}

	var6 = m_field_4170_h->nextInt(m_field_4170_h->nextInt(10) + 1) + 1;

	int var10;
	for(var7 = 0; var7 < var6; ++var7) {
		var8 = var4 + m_field_4170_h->nextInt(16) + 8;
		var9 = m_field_4170_h->nextInt(120) + 4;
		var10 = var5 + m_field_4170_h->nextInt(16) + 8;
		(new WorldGenFire())->generate(m_field_4164_n, m_field_4170_h, var8, var9, var10);
	}

	var6 = m_field_4170_h->nextInt(m_field_4170_h->nextInt(10) + 1);

	for(var7 = 0; var7 < var6; ++var7) {
		var8 = var4 + m_field_4170_h->nextInt(16) + 8;
		var9 = m_field_4170_h->nextInt(120) + 4;
		var10 = var5 + m_field_4170_h->nextInt(16) + 8;
		(new WorldGenLightStone1())->generate(m_field_4164_n, m_field_4170_h, var8, var9, var10);
	}

	for(var7 = 0; var7 < 10; ++var7) {
		var8 = var4 + m_field_4170_h->nextInt(16) + 8;
		var9 = m_field_4170_h->nextInt(128);
		var10 = var5 + m_field_4170_h->nextInt(16) + 8;
		(new WorldGenLightStone2())->generate(m_field_4164_n, m_field_4170_h, var8, var9, var10);
	}

	if(m_field_4170_h->nextInt(1) == 0) {
		var7 = var4 + m_field_4170_h->nextInt(16) + 8;
		var8 = m_field_4170_h->nextInt(128);
		var9 = var5 + m_field_4170_h->nextInt(16) + 8;
		(new WorldGenFlowers(Block::s_mushroomBrown->blockID))->generate(m_field_4164_n, m_field_4170_h, var7, var8, var9);
	}

	if(m_field_4170_h->nextInt(1) == 0) {
		var7 = var4 + m_field_4170_h->nextInt(16) + 8;
		var8 = m_field_4170_h->nextInt(128);
		var9 = var5 + m_field_4170_h->nextInt(16) + 8;
		(new WorldGenFlowers(Block::s_mushroomRed->blockID))->generate(m_field_4164_n, m_field_4170_h, var7, var8, var9);
	}

	BlockSand::s_fallInstantly = false;
}

bool ChunkProviderHell::saveChunks(bool var1, IProgressUpdate* var2) {
	return true;
}

bool ChunkProviderHell::func_532_a() {
	return false;
}

bool ChunkProviderHell::func_536_b() {
	return true;
}
