#include "stdafx.h"
// Auto-generated from MetadataChunkBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MetadataChunkBlock.h"
#include "Common.h"
#include "EnumSkyBlock.h"
#include "World.h"
#include "Block.h"

MetadataChunkBlock::MetadataChunkBlock()
	: m_field_1299_a(NULL)
	, m_field_1298_b(0)
	, m_field_1304_c(0)
	, m_field_1303_d(0)
	, m_field_1302_e(0)
	, m_field_1301_f(0)
	, m_field_1300_g(0)
{
}

MetadataChunkBlock::MetadataChunkBlock(EnumSkyBlock* var1, int var2, int var3, int var4, int var5, int var6, int var7)
	: m_field_1299_a(NULL)
	, m_field_1298_b(0)
	, m_field_1304_c(0)
	, m_field_1303_d(0)
	, m_field_1302_e(0)
	, m_field_1301_f(0)
	, m_field_1300_g(0)
{
		m_field_1299_a = var1;
		m_field_1298_b = var2;
		m_field_1304_c = var3;
		m_field_1303_d = var4;
		m_field_1302_e = var5;
		m_field_1301_f = var6;
		m_field_1300_g = var7;
}

MetadataChunkBlock::~MetadataChunkBlock() {
	m_field_1299_a = NULL;
}

void MetadataChunkBlock::func_4127_a(World* var1) {
		int var2 = m_field_1302_e - m_field_1298_b + 1;
		int var3 = m_field_1301_f - m_field_1304_c + 1;
		int var4 = m_field_1300_g - m_field_1303_d + 1;
		int var5 = var2 * var3 * var4;
		if(var5 <= -(-32768)) {
			for(int var6 = m_field_1298_b; var6 <= m_field_1302_e; ++var6) {
				for(int var7 = m_field_1303_d; var7 <= m_field_1300_g; ++var7) {
					if(var1->blockExists(var6, 0, var7)) {
						for(int var8 = m_field_1304_c; var8 <= m_field_1301_f; ++var8) {
							if(var8 >= 0 && var8 < 128) {
								int var9 = var1->getSavedLightValue(m_field_1299_a, var6, var8, var7);
								bool var10 = false;
								int var11 = var1->getBlockId(var6, var8, var7);
								int var12 = Block::s_lightOpacity[var11];
								if(var12 == 0) {
									var12 = 1;
								}

								int var13 = 0;
								if(m_field_1299_a == EnumSkyBlock::s_Sky) {
									if(var1->canExistingBlockSeeTheSky(var6, var8, var7)) {
										var13 = 15;
									}
								} else if(m_field_1299_a == EnumSkyBlock::s_Block) {
									var13 = Block::s_lightValue[var11];
								}

								int var14;
								int var20;
								if(var12 >= 15 && var13 == 0) {
									var20 = 0;
								} else {
									var14 = var1->getSavedLightValue(m_field_1299_a, var6 - 1, var8, var7);
									int var15 = var1->getSavedLightValue(m_field_1299_a, var6 + 1, var8, var7);
									int var16 = var1->getSavedLightValue(m_field_1299_a, var6, var8 - 1, var7);
									int var17 = var1->getSavedLightValue(m_field_1299_a, var6, var8 + 1, var7);
									int var18 = var1->getSavedLightValue(m_field_1299_a, var6, var8, var7 - 1);
									int var19 = var1->getSavedLightValue(m_field_1299_a, var6, var8, var7 + 1);
									var20 = var14;
									if(var15 > var14) {
										var20 = var15;
									}

									if(var16 > var20) {
										var20 = var16;
									}

									if(var17 > var20) {
										var20 = var17;
									}

									if(var18 > var20) {
										var20 = var18;
									}

									if(var19 > var20) {
										var20 = var19;
									}

									var20 -= var12;
									if(var20 < 0) {
										var20 = 0;
									}

									if(var13 > var20) {
										var20 = var13;
									}
								}

								if(var9 != var20) {
									var1->setLightValue(m_field_1299_a, var6, var8, var7, var20);
									var14 = var20 - 1;
									if(var14 < 0) {
										var14 = 0;
									}

									var1->neighborLightPropagationChanged(m_field_1299_a, var6 - 1, var8, var7, var14);
									var1->neighborLightPropagationChanged(m_field_1299_a, var6, var8 - 1, var7, var14);
									var1->neighborLightPropagationChanged(m_field_1299_a, var6, var8, var7 - 1, var14);
									if(var6 + 1 >= m_field_1302_e) {
										var1->neighborLightPropagationChanged(m_field_1299_a, var6 + 1, var8, var7, var14);
									}

									if(var8 + 1 >= m_field_1301_f) {
										var1->neighborLightPropagationChanged(m_field_1299_a, var6, var8 + 1, var7, var14);
									}

									if(var7 + 1 >= m_field_1300_g) {
										var1->neighborLightPropagationChanged(m_field_1299_a, var6, var8, var7 + 1, var14);
									}
								}
							}
						}
					}
				}
			}

		}
}

bool MetadataChunkBlock::func_866_a(int var1, int var2, int var3, int var4, int var5, int var6) {
		if(var1 >= m_field_1298_b && var2 >= m_field_1304_c && var3 >= m_field_1303_d && var4 <= m_field_1302_e && var5 <= m_field_1301_f && var6 <= m_field_1300_g) {
			return true;
		} else {
			unsigned char var7 = 1;
			if(var1 >= m_field_1298_b - var7 && var2 >= m_field_1304_c - var7 && var3 >= m_field_1303_d - var7 && var4 <= m_field_1302_e + var7 && var5 <= m_field_1301_f + var7 && var6 <= m_field_1300_g + var7) {
				int var8 = m_field_1302_e - m_field_1298_b;
				int var9 = m_field_1301_f - m_field_1304_c;
				int var10 = m_field_1300_g - m_field_1303_d;
				if(var1 > m_field_1298_b) {
					var1 = m_field_1298_b;
				}

				if(var2 > m_field_1304_c) {
					var2 = m_field_1304_c;
				}

				if(var3 > m_field_1303_d) {
					var3 = m_field_1303_d;
				}

				if(var4 < m_field_1302_e) {
					var4 = m_field_1302_e;
				}

				if(var5 < m_field_1301_f) {
					var5 = m_field_1301_f;
				}

				if(var6 < m_field_1300_g) {
					var6 = m_field_1300_g;
				}

				int var11 = var4 - var1;
				int var12 = var5 - var2;
				int var13 = var6 - var3;
				int var14 = var8 * var9 * var10;
				int var15 = var11 * var12 * var13;
				if(var15 - var14 <= 2) {
					m_field_1298_b = var1;
					m_field_1304_c = var2;
					m_field_1303_d = var3;
					m_field_1302_e = var4;
					m_field_1301_f = var5;
					m_field_1300_g = var6;
					return true;
				}
			}

			return false;
		}
}
