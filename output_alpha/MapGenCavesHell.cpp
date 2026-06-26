#include "stdafx.h"
// Auto-generated from MapGenCavesHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MapGenCavesHell.h"
#include "Common.h"
#include "BlockGrass.h"
#include "World.h"
#include "Block.h"
#include "Random.h"

MapGenCavesHell::MapGenCavesHell()
{
}

MapGenCavesHell::~MapGenCavesHell() {
	// No dynamic resources
}

void MapGenCavesHell::func_4129_a(int var1, int var2, unsigned char* var3, double var4, double var6, double var8) {
		func_4128_a(var1, var2, var3, var4, var6, var8, 1.0F + m_field_1305_b->nextFloat() * 6.0F, 0.0F, 0.0F, -1, -1, 0.5f);
}

void MapGenCavesHell::func_4128_a(int var1, int var2, unsigned char* var3, double var4, double var6, double var8, float var10, float var11, float var12, int var13, int var14, double var15) {
		double var17 = (double)(var1 * 16 + 8);
		double var19 = (double)(var2 * 16 + 8);
		float var21 = 0.0F;
		float var22 = 0.0F;
		Random* var23 = new Random(m_field_1305_b->nextLong());
		if(var14 <= 0) {
			int var24 = m_field_1306_a * 16 - 16;
			var14 = var24 - var23->nextInt(var24 / 4);
		}

		bool var51 = false;
		if(var13 == -1) {
			var13 = var14 / 2;
			var51 = true;
		}

		int var25 = var23->nextInt(var14 / 2) + var14 / 4;

		for(bool var26 = var23->nextInt(6) == 0; var13 < var14; ++var13) {
			double var27 = 1.5f + (double)(sinf((float)var13 * 3.14159265358979323846f / (float)var14) * var10 * 1.0F);
			double var29 = var27 * var15;
			float var31 = cosf(var12);
			float var32 = sinf(var12);
			var4 += (double)(cosf(var11) * var31);
			var6 += (double)var32;
			var8 += (double)(sinf(var11) * var31);
			if(var26) {
				var12 *= 0.92F;
			} else {
				var12 *= 0.7F;
			}

			var12 += var22 * 0.1F;
			var11 += var21 * 0.1F;
			var22 *= 0.9F;
			var21 *= 12.0F / 16.0F;
			var22 += (var23->nextFloat() - var23->nextFloat()) * var23->nextFloat() * 2.0F;
			var21 += (var23->nextFloat() - var23->nextFloat()) * var23->nextFloat() * 4.0F;
			if(!var51 && var13 == var25 && var10 > 1.0F) {
				func_4128_a(var1, var2, var3, var4, var6, var8, var23->nextFloat() * 0.5F + 0.5F, var11 - 3.14159265358979323846f * 0.5F, var12 / 3.0F, var13, var14, 1.0f);
				func_4128_a(var1, var2, var3, var4, var6, var8, var23->nextFloat() * 0.5F + 0.5F, var11 + 3.14159265358979323846f * 0.5F, var12 / 3.0F, var13, var14, 1.0f);
				return;
			}

			if(var51 || var23->nextInt(4) != 0) {
				double var33 = var4 - var17;
				double var35 = var8 - var19;
				double var37 = (double)(var14 - var13);
				double var39 = (double)(var10 + 2.0F + 16.0F);
				if(var33 * var33 + var35 * var35 - var37 * var37 > var39 * var39) {
					return;
				}

				if(var4 >= var17 - 16.0f - var27 * 2.0f && var8 >= var19 - 16.0f - var27 * 2.0f && var4 <= var17 + 16.0f + var27 * 2.0f && var8 <= var19 + 16.0f + var27 * 2.0f) {
					int var52 = (int)floor(var4 - var27) - var1 * 16 - 1;
					int var34 = (int)floor(var4 + var27) - var1 * 16 + 1;
					int var53 = (int)floor(var6 - var29) - 1;
					int var36 = (int)floor(var6 + var29) + 1;
					int var54 = (int)floor(var8 - var27) - var2 * 16 - 1;
					int var38 = (int)floor(var8 + var27) - var2 * 16 + 1;
					if(var52 < 0) {
						var52 = 0;
					}

					if(var34 > 16) {
						var34 = 16;
					}

					if(var53 < 1) {
						var53 = 1;
					}

					if(var36 > 120) {
						var36 = 120;
					}

					if(var54 < 0) {
						var54 = 0;
					}

					if(var38 > 16) {
						var38 = 16;
					}

					bool var55 = false;

					int var40;
					int var43;
					for(var40 = var52; !var55 && var40 < var34; ++var40) {
						for(int var41 = var54; !var55 && var41 < var38; ++var41) {
							for(int var42 = var36 + 1; !var55 && var42 >= var53 - 1; --var42) {
								var43 = (var40 * 16 + var41) * 128 + var42;
								if(var42 >= 0 && var42 < 128) {
									if(var3[var43] == Block::s_lavaStill->blockID || var3[var43] == Block::s_lavaMoving->blockID) {
										var55 = true;
									}

									if(var42 != var53 - 1 && var40 != var52 && var40 != var34 - 1 && var41 != var54 && var41 != var38 - 1) {
										var42 = var53;
									}
								}
							}
						}
					}

					if(!var55) {
						for(var40 = var52; var40 < var34; ++var40) {
							double var56 = ((double)(var40 + var1 * 16) + 0.5f - var4) / var27;

							for(var43 = var54; var43 < var38; ++var43) {
								double var44 = ((double)(var43 + var2 * 16) + 0.5f - var8) / var27;
								int var46 = (var40 * 16 + var43) * 128 + var36;

								for(int var47 = var36 - 1; var47 >= var53; --var47) {
									double var48 = ((double)var47 + 0.5f - var6) / var29;
									if(var48 > -0.7f && var56 * var56 + var48 * var48 + var44 * var44 < 1.0f) {
										unsigned char var50 = var3[var46];
										if(var50 == Block::s_bloodStone->blockID || var50 == Block::s_dirt->blockID || var50 == Block::s_grass->blockID) {
											var3[var46] = 0;
										}
									}

									--var46;
								}
							}
						}

						if(var51) {
							break;
						}
					}
				}
			}
		}

}

void MapGenCavesHell::func_868_a(World* var1, int var2, int var3, int var4, int var5, unsigned char* var6) {
		int var7 = m_field_1305_b->nextInt(m_field_1305_b->nextInt(m_field_1305_b->nextInt(10) + 1) + 1);
		if(m_field_1305_b->nextInt(5) != 0) {
			var7 = 0;
		}

		for(int var8 = 0; var8 < var7; ++var8) {
			double var9 = (double)(var2 * 16 + m_field_1305_b->nextInt(16));
			double var11 = (double)m_field_1305_b->nextInt(128);
			double var13 = (double)(var3 * 16 + m_field_1305_b->nextInt(16));
			int var15 = 1;
			if(m_field_1305_b->nextInt(4) == 0) {
				func_4129_a(var4, var5, var6, var9, var11, var13);
				var15 += m_field_1305_b->nextInt(4);
			}

			for(int var16 = 0; var16 < var15; ++var16) {
				float var17 = m_field_1305_b->nextFloat() * 3.14159265358979323846f * 2.0F;
				float var18 = (m_field_1305_b->nextFloat() - 0.5F) * 2.0F / 8.0F;
				float var19 = m_field_1305_b->nextFloat() * 2.0F + m_field_1305_b->nextFloat();
				func_4128_a(var4, var5, var6, var9, var11, var13, var19 * 2.0F, var17, var18, 0, 0, 0.5f);
			}
		}

}
