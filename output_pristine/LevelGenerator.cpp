#include "stdafx.h"
// Auto-generated from LevelGenerator.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LevelGenerator.h"
#include "Common.h"
#include "BlockFlower.h"
#include "IProgressUpdate.h"
#include "Random.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "MobSpawner.h"
#include "NoiseGeneratorDistort.h"
#include "NoiseGeneratorOctaves.h"

LevelGenerator::LevelGenerator()
	: m_guiLoading(NULL)
	, width(0)
	, m_depth(0)
	, height(0)
	, rand(new Random())
	, m_blocksByteArray(0)
	, m_waterLevel(0)
	, m_groundLevel(0)
	, m_islandGen(false)
	, m_floatingGen(false)
	, m_flatGen(false)
	, m_levelType(0)
	, m_phaseBar(0)
	, m_phases(0)
	, m_phaseBareLength(0.0f)
	, m_floodFillBlocks(new int[1048576])
{
}

LevelGenerator::LevelGenerator(IProgressUpdate* var1)
	: m_guiLoading(NULL)
	, width(0)
	, m_depth(0)
	, height(0)
	, rand(new Random())
	, m_blocksByteArray(0)
	, m_waterLevel(0)
	, m_groundLevel(0)
	, m_islandGen(false)
	, m_floatingGen(false)
	, m_flatGen(false)
	, m_levelType(0)
	, m_phaseBar(0)
	, m_phases(0)
	, m_phaseBareLength(0.0f)
	, m_floodFillBlocks(new int[1048576])
{
		m_guiLoading = var1;
}

LevelGenerator::~LevelGenerator() {
	m_guiLoading = NULL;
	rand = NULL;
	delete[] m_blocksByteArray;
	m_blocksByteArray = NULL;
	delete[] m_floodFillBlocks;
	m_floodFillBlocks = NULL;
}

World* LevelGenerator::generate(const std::string& var1, int var2, int var3, int var4) {
		int var5 = 1;
		if(m_floatingGen) {
			var5 = (var4 - 64) / 48 + 1;
		}

		m_phases = 13 + var5 * 4;
		m_guiLoading->displayProgressMessage("Generating level");
		World* var6 = new World();
		var6->m_waterLevel = m_waterLevel;
		var6->m_groundLevel = m_groundLevel;
		width = var2;
		m_depth = var3;
		height = var4;
		m_blocksByteArray = new unsigned char[var2 * var3 * var4];

		int var7;
		LevelGenerator var9;
		int var20;
		int var24;
		int var30;
		int var44;
		int var49;
		int var50;
		int var51;
		int var54;
		for(var7 = 0; var7 < var5; ++var7) {
			m_waterLevel = var4 - 32 - var7 * 48;
			m_groundLevel = m_waterLevel - 2;
			int[] var8;
			NoiseGeneratorOctaves var13;
			double var31;
			int[] var45;
			if(m_flatGen) {
				var8 = new int[var2 * var3];

				for(var44 = 0; var44 < var8_count /* PORT: set this from allocation */; ++var44) {
					var8[var44] = 0;
				}

				loadingBar();
				loadingBar();
			} else {
				m_guiLoading->displayLoadingString("Raising..");
				loadingBar();
				var9 = this;
				NoiseGeneratorDistort* var10 = new NoiseGeneratorDistort(new NoiseGeneratorOctaves(rand, 8), new NoiseGeneratorOctaves(rand, 8));
				NoiseGeneratorDistort* var11 = new NoiseGeneratorDistort(new NoiseGeneratorOctaves(rand, 8), new NoiseGeneratorOctaves(rand, 8));
				NoiseGeneratorOctaves* var12 = new NoiseGeneratorOctaves(rand, 6);
				var13 = new NoiseGeneratorOctaves(rand, 2);
				int* var14 = new int[width * m_depth];
				int var21 = 0;

				label298:
				while(true) {
					if(var21 >= var9.width) {
						var8 = var14;
						m_guiLoading->displayLoadingString("Eroding..");
						loadingBar();
						var45 = var14;
						var9 = this;
						var11 = new NoiseGeneratorDistort(new NoiseGeneratorOctaves(rand, 8), new NoiseGeneratorOctaves(rand, 8));
						NoiseGeneratorDistort* var48 = new NoiseGeneratorDistort(new NoiseGeneratorOctaves(rand, 8), new NoiseGeneratorOctaves(rand, 8));
						var50 = 0;

						while(true) {
							if(var50 >= var9.width) {
								break label298;
							}

							var9.setNextPhase((float)var50 * 100.0F / (float)(var9.width - 1));

							for(var51 = 0; var51 < var9.m_depth; ++var51) {
								double var19 = var11->generateNoise((double)(var50 << 1), (double)(var51 << 1)) / 8.0f;
								var21 = var48->generateNoise((double)(var50 << 1), (double)(var51 << 1)) > 0.0f ? 1 : 0;
								if(var19 > 2.0f) {
									int var56 = var45[var50 + var51 * var9.width];
									var56 = ((var56 - var21) / 2 << 1) + var21;
									var45[var50 + var51 * var9.width] = var56;
								}
							}

							++var50;
						}
					}

					double var22 = abs(((double)var21 / ((double)var9.width - 1.0f) - 0.5f) * 2.0f);
					var9.setNextPhase((float)var21 * 100.0F / (float)(var9.width - 1));

					for(var24 = 0; var24 < var9.m_depth; ++var24) {
						double var25 = abs(((double)var24 / ((double)var9.m_depth - 1.0f) - 0.5f) * 2.0f);
						double var27 = var10->generateNoise((double)((float)var21 * 1.3F), (double)((float)var24 * 1.3F)) / 6.0f + -4.0f;
						double var29 = var11->generateNoise((double)((float)var21 * 1.3F), (double)((float)var24 * 1.3F)) / 5.0f + 10.0f + -4.0f;
						var31 = var12->generateNoise((double)var21, (double)var24) / 8.0f;
						if(var31 > 0.0f) {
							var29 = var27;
						}

						double var33 = max(var27, var29) / 2.0f;
						if(var9.m_islandGen) {
							double var35 = sqrtf(var22 * var22 + var25 * var25) * (double)1.2F;
							double var38 = var13->generateNoise((double)((float)var21 * 0.05F), (double)((float)var24 * 0.05F)) / 4.0f + 1.0f;
							var35 = min(var35, var38);
							var35 = max(var35, max(var22, var25));
							if(var35 > 1.0f) {
								var35 = 1.0f;
							}

							if(var35 < 0.0f) {
								var35 = 0.0f;
							}

							var35 *= var35;
							var33 = var33 * (1.0f - var35) - var35 * 10.0f + 5.0f;
							if(var33 < 0.0f) {
								var33 -= var33 * var33 * (double)0.2F;
							}
						} else if(var33 < 0.0f) {
							var33 *= 0.8f;
						}

						var14[var21 + var24 * var9.width] = (int)var33;
					}

					++var21;
				}
			}

			m_guiLoading->displayLoadingString("Soiling..");
			loadingBar();
			var45 = var8;
			var9 = this;
			int var47 = width;
			var49 = m_depth;
			var50 = height;
			NoiseGeneratorOctaves* var52 = new NoiseGeneratorOctaves(rand, 8);
			NoiseGeneratorOctaves* var53 = new NoiseGeneratorOctaves(rand, 8);

			int var23;
			for(var20 = 0; var20 < var47; ++var20) {
				double var55 = abs(((double)var20 / ((double)var47 - 1.0f) - 0.5f) * 2.0f);
				var9.setNextPhase((float)var20 * 100.0F / (float)(var47 - 1));

				for(var23 = 0; var23 < var49; ++var23) {
					double var62 = abs(((double)var23 / ((double)var49 - 1.0f) - 0.5f) * 2.0f);
					double var26 = max(var55, var62);
					var26 = var26 * var26 * var26;
					int var28 = (int)(var52->generateNoise((double)var20, (double)var23) / 24.0f) - 4;
					int var69 = var45[var20 + var23 * var47] + var9.m_waterLevel;
					var30 = var69 + var28;
					var45[var20 + var23 * var47] = max(var69, var30);
					if(var45[var20 + var23 * var47] > var50 - 2) {
						var45[var20 + var23 * var47] = var50 - 2;
					}

					if(var45[var20 + var23 * var47] <= 0) {
						var45[var20 + var23 * var47] = 1;
					}

					var31 = var53->generateNoise((double)var20 * 2.3f, (double)var23 * 2.3f) / 24.0f;
					int var73 = (int)(sqrtf(abs(var31)) * signumf(var31) * 20.0f) + var9.m_waterLevel;
					var73 = (int)((double)var73 * (1.0f - var26) + var26 * (double)var9.height);
					if(var73 > var9.m_waterLevel) {
						var73 = var9.height;
					}

					for(int var34 = 0; var34 < var50; ++var34) {
						int var76 = (var34 * var9.m_depth + var23) * var9.width + var20;
						int var36 = 0;
						if(var34 <= var69) {
							var36 = Block::s_dirt->blockID;
						}

						if(var34 <= var30) {
							var36 = Block::s_stone->blockID;
						}

						if(var9.m_floatingGen && var34 < var73) {
							var36 = 0;
						}

						if(var9.m_blocksByteArray[var76] == 0) {
							var9.m_blocksByteArray[var76] = (unsigned char)var36;
						}
					}
				}
			}

			m_guiLoading->displayLoadingString("Growing..");
			loadingBar();
			var45 = var8;
			var9 = this;
			var47 = width;
			var49 = m_depth;
			var13 = new NoiseGeneratorOctaves(rand, 8);
			var52 = new NoiseGeneratorOctaves(rand, 8);

			for(var54 = 0; var54 < var47; ++var54) {
				var9.setNextPhase((float)var54 * 100.0F / (float)(var47 - 1));

				for(var20 = 0; var20 < var49; ++var20) {
					bool var57 = var13->generateNoise((double)var54, (double)var20) > 8.0f;
					if(var9.m_islandGen) {
						var57 = var13->generateNoise((double)var54, (double)var20) > -8.0f;
					}

					bool var58 = var52->generateNoise((double)var54, (double)var20) > 12.0f;
					var23 = var45[var54 + var20 * var47];
					var24 = (var23 * var9.m_depth + var20) * var9.width + var54;
					int var63 = var9.m_blocksByteArray[((var23 + 1) * var9.m_depth + var20) * var9.width + var54] & 255;
					if((var63 == Block::s_waterMoving->blockID || var63 == Block::s_waterStill->blockID) && var23 <= var9.m_waterLevel - 1 && var58) {
						var9.m_blocksByteArray[var24] = (unsigned char)Block::s_gravel->blockID;
					}

					if(var63 == 0) {
						int var65 = -1;
						if(var23 <= var9.m_waterLevel - 1 && var57) {
							var65 = Block::s_sand->blockID;
						}

						if(var9.m_blocksByteArray[var24] != 0 && var65 > 0) {
							var9.m_blocksByteArray[var24] = (unsigned char)var65;
						}
					}
				}
			}
		}

		m_guiLoading->displayLoadingString("Carving..");
		loadingBar();
		var9 = this;
		var49 = width;
		var50 = m_depth;
		var51 = height;
		var54 = var49 * var50 * var51 / 256 / 64 << 1;

		for(var20 = 0; var20 < var54; ++var20) {
			var9.setNextPhase((float)var20 * 100.0F / (float)(var54 - 1));
			float var59 = var9.rand->nextFloat() * (float)var49;
			float var60 = var9.rand->nextFloat() * (float)var51;
			float var61 = var9.rand->nextFloat() * (float)var50;
			var24 = (int)((var9.rand->nextFloat() + var9.rand->nextFloat()) * 200.0F);
			float var64 = var9.rand->nextFloat() * 3.14159265358979323846f * 2.0F;
			float var67 = 0.0F;
			float var66 = var9.rand->nextFloat() * 3.14159265358979323846f * 2.0F;
			float var68 = 0.0F;
			float var70 = var9.rand->nextFloat() * var9.rand->nextFloat();

			for(var30 = 0; var30 < var24; ++var30) {
				var59 += sinf(var64) * cosf(var66);
				var61 += cosf(var64) * cosf(var66);
				var60 += sinf(var66);
				var64 += var67 * 0.2F;
				var67 *= 0.9F;
				var67 += var9.rand->nextFloat() - var9.rand->nextFloat();
				var66 += var68 * 0.5F;
				var66 *= 0.5F;
				var68 *= 12.0F / 16.0F;
				var68 += var9.rand->nextFloat() - var9.rand->nextFloat();
				if(var9.rand->nextFloat() >= 0.25F) {
					float var71 = var59 + (var9.rand->nextFloat() * 4.0F - 2.0F) * 0.2F;
					float var32 = var60 + (var9.rand->nextFloat() * 4.0F - 2.0F) * 0.2F;
					float var74 = var61 + (var9.rand->nextFloat() * 4.0F - 2.0F) * 0.2F;
					float var72 = ((float)var9.height - var32) / (float)var9.height;
					float var77 = 1.2F + (var72 * 3.5F + 1.0F) * var70;
					float var75 = sinf((float)var30 * 3.14159265358979323846f / (float)var24) * var77;

					for(var5 = (int)(var71 - var75); var5 <= (int)(var71 + var75); ++var5) {
						for(int var78 = (int)(var32 - var75); var78 <= (int)(var32 + var75); ++var78) {
							for(int var39 = (int)(var74 - var75); var39 <= (int)(var74 + var75); ++var39) {
								float var40 = (float)var5 - var71;
								float var42 = (float)var78 - var32;
								float var46 = (float)var39 - var74;
								var40 = var40 * var40 + var42 * var42 * 2.0F + var46 * var46;
								if(var40 < var75 * var75 && var5 > 0 && var78 > 0 && var39 > 0 && var5 < var9.width - 1 && var78 < var9.height - 1 && var39 < var9.m_depth - 1) {
									var7 = (var78 * var9.m_depth + var39) * var9.width + var5;
									if(var9.m_blocksByteArray[var7] == Block::s_stone->blockID) {
										var9.m_blocksByteArray[var7] = 0;
									}
								}
							}
						}
					}
				}
			}
		}

		var7 = populateOre(Block::s_oreCoal->blockID, 1000, 10, (var4 << 2) / 5);
		int var43 = populateOre(Block::s_oreIron->blockID, 800, 8, var4 * 3 / 5);
		var44 = populateOre(Block::s_oreGold->blockID, 500, 6, (var4 << 1) / 5);
		var5 = populateOre(Block::s_oreDiamond->blockID, 800, 2, var4 / 5);
		DbgPrintF("%s\n", "Coal: " + var7 + ", Iron: " + var43 + ", Gold: " + var44 + ", Diamond: " + var5);
		m_guiLoading->displayLoadingString("Melting..");
		loadingBar();
		lavaGen();
		var6->m_cloudHeight = var4 + 2;
		if(m_floatingGen) {
			m_groundLevel = -128;
			m_waterLevel = m_groundLevel + 1;
			var6->m_cloudHeight = -16;
		} else if(!m_islandGen) {
			m_groundLevel = m_waterLevel + 1;
			m_waterLevel = m_groundLevel - 16;
		} else {
			m_groundLevel = m_waterLevel - 9;
		}

		m_guiLoading->displayLoadingString("Watering..");
		loadingBar();
		liquidThemeSpawner();
		if(!m_floatingGen) {
			var5 = Block::s_waterStill->blockID;
			if(m_levelType == 1) {
				var5 = Block::s_lavaStill->blockID;
			}

			for(var7 = 0; var7 < var2; ++var7) {
				floodFill(var7, m_waterLevel - 1, 0, 0, var5);
				floodFill(var7, m_waterLevel - 1, var3 - 1, 0, var5);
			}

			for(var7 = 0; var7 < var3; ++var7) {
				floodFill(var2 - 1, m_waterLevel - 1, var7, 0, var5);
				floodFill(0, m_waterLevel - 1, var7, 0, var5);
			}
		}

		if(m_levelType == 1) {
			var6->m_cloudColor = 2164736;
			var6->m_fogColor = 1049600;
			var6->m_skyColor = 1049600;
			var6->m_skylightSubtracted = var6->m_skyBrightness = 7;
			var6->m_defaultFluid = Block::s_lavaMoving->blockID;
			if(m_floatingGen) {
				var6->m_cloudHeight = var4 + 2;
				m_waterLevel = -16;
			}
		}

		var6->m_waterLevel = m_waterLevel;
		var6->m_groundLevel = m_groundLevel;
		m_guiLoading->displayLoadingString("Assembling..");
		loadingBar();
		setNextPhase(0.0F);
		var6->generate(var2, var4, var3, m_blocksByteArray);
		m_guiLoading->displayLoadingString("Building..");
		loadingBar();
		setNextPhase(0.0F);
		var6->findSpawn();
		generateHouse(var6);
		m_guiLoading->displayLoadingString("Planting..");
		loadingBar();
		if(m_levelType != 1) {
			growGrassOnDirt(var6);
		}

		loadingBar();
		growTrees(var6);
		loadingBar();
		populateFlowersAndMushrooms(var6, Block.plantYellow, 100);
		loadingBar();
		populateFlowersAndMushrooms(var6, Block.plantRed, 100);
		loadingBar();
		populateFlowersAndMushrooms(var6, Block::s_mushroomBrown, 50);
		loadingBar();
		populateFlowersAndMushrooms(var6, Block::s_mushroomRed, 50);
		m_guiLoading->displayLoadingString("Lighting..");
		loadingBar();

		for(var5 = 0; var5 < 10000; ++var5) {
			setNextPhase((float)(var5 * 100 / 10000));
			var6->updateLighting();
		}

		m_guiLoading->displayLoadingString("Spawning..");
		loadingBar();
		MobSpawner* var41 = new MobSpawner(var6);

		for(var7 = 0; var7 < 1000; ++var7) {
			setNextPhase((float)var7 * 100.0F / 999.0F);
			var41->performSpawning();
		}

		var6->m_createTime = GetTickCount();
		var6->m_authorName = var1;
		var6->m_name = "A Nice World";
		if(m_phaseBar != m_phases) {
			DbgPrintF("ERROR [IllegalStateException]: %s\n", "Wrong number of phases! Wanted " + this.m_phases + ", got " + this.m_phaseBar);
		} else {
			return var6;
		}
}

void LevelGenerator::generateHouse(World* var0) {
		int var1 = var0->m_xSpawn;
		int var2 = var0->m_ySpawn;
		int var3 = var0->m_zSpawn;

		for(int var4 = var1 - 3; var4 <= var1 + 3; ++var4) {
			for(int var5 = var2 - 2; var5 <= var2 + 2; ++var5) {
				for(int var6 = var3 - 3; var6 <= var3 + 3; ++var6) {
					int var7 = var5 < var2 - 1 ? Block::s_obsidian->blockID : 0;
					if(var4 == var1 - 3 || var6 == var3 - 3 || var4 == var1 + 3 || var6 == var3 + 3 || var5 == var2 - 2 || var5 == var2 + 2) {
						var7 = Block::s_stone->blockID;
						if(var5 >= var2 - 1) {
							var7 = Block::s_planks->blockID;
						}
					}

					if(var6 == var3 - 3 && var4 == var1 && var5 >= var2 - 1 && var5 <= var2) {
						var7 = 0;
					}

					var0->setBlockWithNotify(var4, var5, var6, var7);
				}
			}
		}

		var0->setBlockWithNotify(var1 - 3 + 1, var2, var3, Block.torch.blockID);
		var0->setBlockWithNotify(var1 + 3 - 1, var2, var3, Block.torch.blockID);
}

void LevelGenerator::growGrassOnDirt(World* var1) {
		for(int var2 = 0; var2 < width; ++var2) {
			setNextPhase((float)var2 * 100.0F / (float)(width - 1));

			for(int var3 = 0; var3 < height; ++var3) {
				for(int var4 = 0; var4 < m_depth; ++var4) {
					if(var1->getBlockId(var2, var3, var4) == Block::s_dirt->blockID && var1->getBlockLightValue(var2, var3 + 1, var4) >= 4 && !var1->getBlockMaterial(var2, var3 + 1, var4).getCanBlockGrass()) {
						var1->setBlock(var2, var3, var4, Block::s_grass->blockID);
					}
				}
			}
		}

}

void LevelGenerator::growTrees(World* var1) {
		int var2 = width * m_depth * height / 80000;

		for(int var3 = 0; var3 < var2; ++var3) {
			if(var3 % 100 == 0) {
				setNextPhase((float)var3 * 100.0F / (float)(var2 - 1));
			}

			int var4 = rand->nextInt(width);
			int var5 = rand->nextInt(height);
			int var6 = rand->nextInt(m_depth);

			for(int var7 = 0; var7 < 25; ++var7) {
				int var8 = var4;
				int var9 = var5;
				int var10 = var6;

				for(int var11 = 0; var11 < 20; ++var11) {
					var8 += rand->nextInt(12) - rand->nextInt(12);
					var9 += rand->nextInt(3) - rand->nextInt(6);
					var10 += rand->nextInt(12) - rand->nextInt(12);
					if(var8 >= 0 && var9 >= 0 && var10 >= 0 && var8 < width && var9 < height && var10 < m_depth) {
						var1->growTrees(var8, var9, var10);
					}
				}
			}
		}

}

void LevelGenerator::populateFlowersAndMushrooms(World* var1, BlockFlower* var2, int var3) {
		var3 = width * m_depth * height * var3 / 1600000;

		for(int var4 = 0; var4 < var3; ++var4) {
			if(var4 % 100 == 0) {
				setNextPhase((float)var4 * 100.0F / (float)(var3 - 1));
			}

			int var5 = rand->nextInt(width);
			int var6 = rand->nextInt(height);
			int var7 = rand->nextInt(m_depth);

			for(int var8 = 0; var8 < 10; ++var8) {
				int var9 = var5;
				int var10 = var6;
				int var11 = var7;

				for(int var12 = 0; var12 < 10; ++var12) {
					var9 += rand->nextInt(4) - rand->nextInt(4);
					var10 += rand->nextInt(2) - rand->nextInt(2);
					var11 += rand->nextInt(4) - rand->nextInt(4);
					if(var9 >= 0 && var11 >= 0 && var10 > 0 && var9 < width && var11 < m_depth && var10 < height && var1->getBlockId(var9, var10, var11) == 0 && var2->canBlockStay(var1, var9, var10, var11)) {
						var1->setBlockWithNotify(var9, var10, var11, var2->blockID);
					}
				}
			}
		}

}

int LevelGenerator::populateOre(int var1, int var2, int var3, int var4) {
		int var5 = 0;
		unsigned char var26 = (unsigned char)var1;
		int var6 = width;
		int var7 = m_depth;
		int var8 = height;
		var2 = var6 * var7 * var8 / 256 / 64 * var2 / 100;

		for(int var9 = 0; var9 < var2; ++var9) {
			setNextPhase((float)var9 * 100.0F / (float)(var2 - 1));
			float var10 = rand->nextFloat() * (float)var6;
			float var11 = rand->nextFloat() * (float)var8;
			float var12 = rand->nextFloat() * (float)var7;
			if(var11 <= (float)var4) {
				int var13 = (int)((rand->nextFloat() + rand->nextFloat()) * 75.0F * (float)var3 / 100.0F);
				float var14 = rand->nextFloat() * 3.14159265358979323846f * 2.0F;
				float var15 = 0.0F;
				float var16 = rand->nextFloat() * 3.14159265358979323846f * 2.0F;
				float var17 = 0.0F;

				for(int var18 = 0; var18 < var13; ++var18) {
					var10 += sinf(var14) * cosf(var16);
					var12 += cosf(var14) * cosf(var16);
					var11 += sinf(var16);
					var14 += var15 * 0.2F;
					var15 *= 0.9F;
					var15 += rand->nextFloat() - rand->nextFloat();
					var16 += var17 * 0.5F;
					var16 *= 0.5F;
					var17 *= 0.9F;
					var17 += rand->nextFloat() - rand->nextFloat();
					float var19 = sinf((float)var18 * 3.14159265358979323846f / (float)var13) * (float)var3 / 100.0F + 1.0F;

					for(int var20 = (int)(var10 - var19); var20 <= (int)(var10 + var19); ++var20) {
						for(int var21 = (int)(var11 - var19); var21 <= (int)(var11 + var19); ++var21) {
							for(int var22 = (int)(var12 - var19); var22 <= (int)(var12 + var19); ++var22) {
								float var23 = (float)var20 - var10;
								float var24 = (float)var21 - var11;
								float var25 = (float)var22 - var12;
								var23 = var23 * var23 + var24 * var24 * 2.0F + var25 * var25;
								if(var23 < var19 * var19 && var20 > 0 && var21 > 0 && var22 > 0 && var20 < width - 1 && var21 < height - 1 && var22 < m_depth - 1) {
									int var27 = (var21 * m_depth + var22) * width + var20;
									if(m_blocksByteArray[var27] == Block::s_stone->blockID) {
										m_blocksByteArray[var27] = var26;
										++var5;
									}
								}
							}
						}
					}
				}
			}
		}

		return var5;
}

void LevelGenerator::liquidThemeSpawner() {
		int var1 = Block::s_waterStill->blockID;
		if(m_levelType == 1) {
			var1 = Block::s_lavaStill->blockID;
		}

		int var2 = width * m_depth * height / 1000;

		for(int var3 = 0; var3 < var2; ++var3) {
			if(var3 % 100 == 0) {
				setNextPhase((float)var3 * 100.0F / (float)(var2 - 1));
			}

			int var4 = rand->nextInt(width);
			int var5 = rand->nextInt(height);
			int var6 = rand->nextInt(m_depth);
			if(m_blocksByteArray[(var5 * m_depth + var6) * width + var4] == 0) {
				long var7 = floodFill(var4, var5, var6, 0, 255);
				if(var7 > 0L && var7 < 640L) {
					floodFill(var4, var5, var6, 255, var1);
				} else {
					floodFill(var4, var5, var6, 255, 0);
				}
			}
		}

		setNextPhase(100.0F);
}

void LevelGenerator::loadingBar() {
		++m_phaseBar;
		m_phaseBareLength = 0.0F;
		setNextPhase(0.0F);
}

void LevelGenerator::setNextPhase(float var1) {
		if(var1 < 0.0F) {
			DbgPrintF("ERROR [IllegalStateException]: %s\n", "Failed to set next phase!");
		} else {
			int var2 = (int)(((float)(m_phaseBar - 1) + var1 / 100.0F) * 100.0F / (float)m_phases);
			m_guiLoading->setLoadingProgress(var2);
		}
}

void LevelGenerator::lavaGen() {
		int var1 = width * m_depth * height / 2000;
		int var2 = m_groundLevel;

		for(int var3 = 0; var3 < var1; ++var3) {
			if(var3 % 100 == 0) {
				setNextPhase((float)var3 * 100.0F / (float)(var1 - 1));
			}

			int var4 = rand->nextInt(width);
			int var5 = min(min(rand->nextInt(var2), rand->nextInt(var2)), min(rand->nextInt(var2), rand->nextInt(var2)));
			int var6 = rand->nextInt(m_depth);
			if(m_blocksByteArray[(var5 * m_depth + var6) * width + var4] == 0) {
				long var7 = floodFill(var4, var5, var6, 0, 255);
				if(var7 > 0L && var7 < 640L) {
					floodFill(var4, var5, var6, 255, Block::s_lavaStill->blockID);
				} else {
					floodFill(var4, var5, var6, 255, 0);
				}
			}
		}

		setNextPhase(100.0F);
}

long long LevelGenerator::floodFill(int var1, int var2, int var3, int var4, int var5) {
		unsigned char var6 = (unsigned char)var5;
		unsigned char var22 = (unsigned char)var4;
		std::vector<void*> var7 /* PORT: determine element type */;
		unsigned char var8 = 0;
		int var9 = 1;

		int var10;
		for(var10 = 1; 1 << var9 < width; ++var9) {
		}

		while(1 << var10 < m_depth) {
			++var10;
		}

		int var11 = m_depth - 1;
		int var12 = width - 1;
		int var23 = var8 + 1;
		m_floodFillBlocks[0] = ((var2 << var10) + var3 << var9) + var1;
		long var14 = 0L;
		var1 = width * m_depth;

		while(var23 > 0) {
			--var23;
			var2 = m_floodFillBlocks[var23];
			if(var23 == 0 && var7.size() > 0) {
				m_floodFillBlocks = (int[])var7.remove(var7.size() - 1);
				var23 = 1048576;
			}

			var3 = var2 >> var9 & var11;
			int var13 = var2 >> var9 + var10;
			int var16 = var2 & var12;

			int var17;
			for(var17 = var16; var16 > 0 && m_blocksByteArray[var2 - 1] == var22; --var2) {
				--var16;
			}

			while(var17 < width && m_blocksByteArray[var2 + var17 - var16] == var22) {
				++var17;
			}

			int var18 = var2 >> var9 & var11;
			int var19 = var2 >> var9 + var10;
			if(var5 == 255 && (var16 == 0 || var17 == width - 1 || var13 == 0 || var13 == height - 1 || var3 == 0 || var3 == m_depth - 1)) {
				return -1L;
			}

			if(var18 != var3 || var19 != var13) {
				DbgPrintF("%s\n", "Diagonal flood!?");
			}

			bool var24 = false;
			bool var25 = false;
			bool var20 = false;
			var14 += (long)(var17 - var16);

			for(var16 = var16; var16 < var17; ++var16) {
				m_blocksByteArray[var2] = var6;
				bool var21;
				if(var3 > 0) {
					var21 = m_blocksByteArray[var2 - width] == var22;
					if(var21 && !var24) {
						if(var23 == 1048576) {
							var7.push_back(m_floodFillBlocks);
							m_floodFillBlocks = new int[1048576];
							var23 = 0;
						}

						m_floodFillBlocks[var23++] = var2 - width;
					}

					var24 = var21;
				}

				if(var3 < m_depth - 1) {
					var21 = m_blocksByteArray[var2 + width] == var22;
					if(var21 && !var25) {
						if(var23 == 1048576) {
							var7.push_back(m_floodFillBlocks);
							m_floodFillBlocks = new int[1048576];
							var23 = 0;
						}

						m_floodFillBlocks[var23++] = var2 + width;
					}

					var25 = var21;
				}

				if(var13 > 0) {
					unsigned char var26 = m_blocksByteArray[var2 - var1];
					if((var6 == Block::s_lavaMoving->blockID || var6 == Block::s_lavaStill->blockID) && (var26 == Block::s_waterMoving->blockID || var26 == Block::s_waterStill->blockID)) {
						m_blocksByteArray[var2 - var1] = (unsigned char)Block::s_stone->blockID;
					}

					var21 = var26 == var22;
					if(var21 && !var20) {
						if(var23 == 1048576) {
							var7.push_back(m_floodFillBlocks);
							m_floodFillBlocks = new int[1048576];
							var23 = 0;
						}

						m_floodFillBlocks[var23++] = var2 - var1;
					}

					var20 = var21;
				}

				++var2;
			}
		}

		return var14;
}
