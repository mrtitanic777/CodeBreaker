#include "stdafx.h"
// Auto-generated from Light.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Light.h"
#include "Common.h"
#include "World.h"
#include <algorithm>
#include <string>
#include "Block.h"
#include "IWorldAccess.h"

Light::Light()
	: m_lightingUpdateCounter(0)
	, worldObj(NULL)
	, m_worldWidth(0)
	, m_worldLength(0)
	, m_worldHeight(0)
	, m_blocks(0)
	, m_data(0)
	, m_heightMap(0)
	, m_lightingUpdateList3(0)
	, m_chunks(0)
	, m_lightValue(0)
	, m_skylightSubtracted(0)
{
}

Light::Light(World* var1)
	: m_lightingUpdateCounter(0)
	, worldObj(NULL)
	, m_worldWidth(0)
	, m_worldLength(0)
	, m_worldHeight(0)
	, m_blocks(0)
	, m_data(0)
	, m_heightMap(0)
	, m_lightingUpdateList3(0)
	, m_chunks(0)
	, m_lightValue(0)
	, m_skylightSubtracted(0)
{
		new LinkedList();
		m_skyLightList = /* default vector */;
		m_blockLightList = /* default vector */;
		m_lightingUpdateList1 = /* default vector */;
		m_lightingUpdateList2 = /* default vector */;
		m_lightingUpdateList3 = getLightingUpdates();
		m_hasMetadataChunkBlock = false;
		m_lightValue = 0;
		worldObj = var1;
		m_worldWidth = var1->width;
		m_worldLength = 4 /* PORT: quad vertex count */;
		m_worldHeight = var1->height;
		m_chunks = new unsigned char[var1->m_d /* PORT: array length - use tracked size */ / 8];
		m_blocks = var1->m_d;
		m_data = var1->m_e;
		m_heightMap = var1->m_heightMap;
}

Light::~Light() {
	worldObj = NULL;
	delete[] m_blocks;
	m_blocks = NULL;
	delete[] m_data;
	m_data = NULL;
	delete[] m_heightMap;
	m_heightMap = NULL;
	delete[] m_lightingUpdateList3;
	m_lightingUpdateList3 = NULL;
	delete[] m_chunks;
	m_chunks = NULL;
}

int* Light::getLightingUpdates() {
		{ auto _it = std::find(m_lightingUpdateList2.begin(), m_lightingUpdateList2.end(), m_lightingUpdateList2.size(); if (_it != m_lightingUpdateList2.end()) m_lightingUpdateList2.erase(_it); }
}

void Light::updateSkylight(int var1, int var2, int var3, int var4) {
		m_lightingUpdateList1.push_back(MetadataChunkBlock(this, var1, var2, 0, var3, var4, 1));
}

void Light::updateDaylightCycle(int var1) {
		if(var1 > 15) {
			var1 = 15;
		}

		if(var1 < 0) {
			var1 = 0;
		}

		m_skylightSubtracted = var1 - worldObj->m_skylightSubtracted;
		if(m_skylightSubtracted != 0) {
			m_lightValue = worldObj->m_skylightSubtracted;
			worldObj->m_skylightSubtracted = var1;

			while(m_hasMetadataChunkBlock) {
				updateLight(64);
			}

			m_metadataChunkBlock = MetadataChunkBlock(this, 0, 0, 0, worldObj->width, worldObj->height, worldObj /* PORT: array length - use tracked size */); m_hasMetadataChunkBlock = true;
		}
}

void Light::updateLight(int var1) {
		int var2 = m_metadataChunkBlock.m_x;
		int var3 = m_metadataChunkBlock.maxX;
		int var4 = m_metadataChunkBlock.m_z;
		int var5 = m_metadataChunkBlock.maxZ;

		int var6;
		int var7;
		for(var2 = var2; var2 < var3; ++var2) {
			if(var1-- <= 0 && var2 != var3 - 1) {
				m_metadataChunkBlock.m_x = var2;
				return;
			}

			for(var6 = var4; var6 < var5; ++var6) {
				for(var7 = m_heightMap[var2 + var6 * m_worldWidth] - 1; var7 > 0 && Block::s_lightOpacity[m_blocks[(var7 * m_worldLength + var6) * m_worldWidth + var2]] < 100; --var7) {
				}

				++var7;

				for(; var7 < m_worldHeight; ++var7) {
					int var8 = (var7 * m_worldLength + var6) * m_worldWidth + var2;
					if(Block::s_lightValue[m_blocks[var8]] == 0) {
						int var9 = m_data[var8] & 15;
						if(var9 <= m_lightValue) {
							if(m_skylightSubtracted < 0 && var9 > 0) {
								--m_data[var8];
							} else if(m_skylightSubtracted > 0 && var9 < 15) {
								++m_data[var8];
							}
						}
					}
				}
			}
		}

		for(var6 = 0; var6 < m_worldWidth; var6 += 32) {
			for(var7 = 0; var7 < m_worldLength; var7 += 32) {
				m_blockLightList.push_back(MetadataChunkBlock(this, var6, 0, var7, var6 + 32, m_worldHeight, var7 + 32));
				m_skyLightList.push_back(MetadataChunkBlock(this, var6, 0, var7, var6 + 32, m_worldHeight, var7 + 32));
			}
		}

		for(var6 = 0; var6 < worldObj->m_worldAccesses.size(); ++var6) {
			IWorldAccess* var10 = (IWorldAccess*)worldObj->m_worldAccesses[var6];
			var10->updateAllRenderers();
		}

		m_hasMetadataChunkBlock = false;
}

void Light::updateBlockLight(int var1, int var2, int var3, int var4, int var5, int var6) {
		m_blockLightList.push_back(MetadataChunkBlock(this, var1, var2, var3, var4, var5, var6));
}

void Light::updateLists(int var1, int var2, int var3, int var4, int var5, int var6) {
		for(var2 = var2; var2 < var5; ++var2) {
			for(int var7 = var3; var7 < var6; ++var7) {
				for(int var8 = var1; var8 < var4; ++var8) {
					int var9 = var8 + var2 * m_worldWidth + var7 * m_worldWidth * m_worldHeight;
					if((m_chunks[var9 >> 3] & 1 << (var9 & 7)) == 0) {
						m_chunks[var9 >> 3] = (unsigned char)(m_chunks[var9 >> 3] | 1 << (var9 & 7));
						m_lightingUpdateList3[m_lightingUpdateCounter++] = var9;
						if((m_chunks[var9 >> 3] & 1 << (var9 & 7)) == 0) {
							DbgPrintF("%s\n", "OMG ERROR!");
						}

						if(m_lightingUpdateCounter > m_lightingUpdateList3 /* PORT: array length - use tracked size */ - 32) {
							var9 = m_lightingUpdateList3[--m_lightingUpdateCounter];
							m_lightingUpdateList3[m_lightingUpdateList3 /* PORT: array length - use tracked size */ - 1] = m_lightingUpdateCounter;
							m_lightingUpdateList.push_back(m_lightingUpdateList3);
							m_lightingUpdateList3 = getLightingUpdates();
							m_lightingUpdateCounter = 1;
							m_lightingUpdateList3[0] = var9;
						}
					}
				}
			}
		}

}

void Light::updateLight() {
		if(m_lightingUpdateList2.size() > 0) {
			{ auto _it = std::find(m_lightingUpdateList2.begin(), m_lightingUpdateList2.end(), m_lightingUpdateList2.size(); if (_it != m_lightingUpdateList2.end()) m_lightingUpdateList2.erase(_it); }
		}

		int var1 = 5;

		while(m_skyLightList.size() > 0 && var1-- > 0) {
   std::vector<void*>::iterator _removeIt = std::find(m_skyLightList.begin(), m_skyLightList.end(), 0);
   if (_removeIt != m_skyLightList.end()) m_skyLightList.erase(_removeIt);

			for(int var3 = 0; var3 < worldObj->m_worldAccesses.size(); ++var3) {
				((IWorldAccess*)worldObj->m_worldAccesses[var3]).markBlockRangeNeedsUpdate(var2.m_x, var2.m_y, var2.m_z, var2.maxX, var2.maxY, var2.maxZ);
			}
		}

		if(m_hasMetadataChunkBlock) {
			updateLight(8);
		} else {
			for(int var19 = 0; var19 < 16; ++var19) {
				bool var20 = false;
				MetadataChunkBlock var21;
				if(m_blockLightList.size() > 0) {
					var20 = true;
     std::vector<void*>::iterator _removeIt = std::find(m_blockLightList.begin(), m_blockLightList.end(), 0);
     if (_removeIt != m_blockLightList.end()) m_blockLightList.erase(_removeIt);
					updateLists(var21.m_x, var21.m_y, var21.m_z, var21.maxX, var21.maxY, var21.maxZ);
				}

				int var4;
				int var5;
				int var6;
				int var7;
				int var8;
				int var9;
				int var10;
				int var11;
				int var12;
				int var13;
				Light* var22;
				if(m_lightingUpdateList1.size() > 0) {
					var20 = true;
     std::vector<void*>::iterator _removeIt = std::find(m_lightingUpdateList1.begin(), m_lightingUpdateList1.end(), 0);
     if (_removeIt != m_lightingUpdateList1.end()) m_lightingUpdateList1.erase(_removeIt);
					var7 = var21.maxY;
					var6 = var21.maxX;
					var5 = var21.m_y;
					var4 = var21.m_x;
					var22 = this;

					for(var8 = var4; var8 < var4 + var6; ++var8) {
						for(var9 = var5; var9 < var5 + var7; ++var9) {
							var10 = var22->m_heightMap[var8 + var9 * var22->m_worldWidth];

							for(var11 = var22->m_worldHeight - 1; var11 > 0 && Block::s_lightOpacity[var22->m_blocks[(var11 * var22->m_worldLength + var9) * var22->m_worldWidth + var8]] == 0; --var11) {
							}

							var22->m_heightMap[var8 + var9 * var22->m_worldWidth] = var11 + 1;
							if(var10 != var11) {
								var12 = var10 < var11 ? var10 : var11;
								var13 = var10 > var11 ? var10 : var11;
								var22->updateLists(var8, var12, var9, var8 + 1, var13, var9 + 1);
							}
						}
					}
				}

				var22 = this;
				var4 = worldObj->m_skylightSubtracted;
				var5 = -999;
				var6 = -999;
				var7 = -999;
				var8 = -999;
				var9 = -999;
				var10 = -999;
				var11 = 1024;
				var12 = 0;

				while(var11-- > 0 && (var22->m_lightingUpdateCounter > 0 || var22->m_lightingUpdateList.size() > 0)) {
					++var12;
					if(var22->m_lightingUpdateCounter == 0) {
						if(var22->m_lightingUpdateList3 != NULL) {
							int[] var23 = var22->m_lightingUpdateList3;
							var22->m_lightingUpdateList2.push_back(var23);
						}

						{ auto _it = std::find(m_lightingUpdateList.begin(), m_lightingUpdateList.end(), var22->m_lightingUpdateList.size(); if (_it != m_lightingUpdateList.end()) m_lightingUpdateList.erase(_it); }
						var22->m_lightingUpdateCounter = var22->m_lightingUpdateList3[var22->m_lightingUpdateList3 /* PORT: array length - use tracked size */ - 1];
					}

					if(var22->m_lightingUpdateCounter > var22->m_lightingUpdateList3 /* PORT: array length - use tracked size */ - 32) {
						var13 = var22->m_lightingUpdateList3[--var22->m_lightingUpdateCounter];
						var22->m_lightingUpdateList3[var22->m_lightingUpdateList3 /* PORT: array length - use tracked size */ - 1] = var22->m_lightingUpdateCounter;
						var22->m_lightingUpdateList.push_back(var22->m_lightingUpdateList3);
						var22->m_lightingUpdateList3 = var22->getLightingUpdates();
						var22->m_lightingUpdateCounter = 1;
						var22->m_lightingUpdateList3[0] = var13;
					} else {
						var13 = var22->m_lightingUpdateList3[--var22->m_lightingUpdateCounter];
						var1 = var13 % var22->m_worldWidth;
						int var14 = var13 / var22->m_worldWidth % var22->m_worldHeight;
						int var15 = var13 / var22->m_worldWidth / var22->m_worldHeight % var22->m_worldLength;
						var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] ^ 1 << (var13 & 7));
						var13 = var22->m_heightMap[var1 + var15 * var22->m_worldWidth];
						int var16 = var14 >= var13 ? var4 : 0;
						unsigned char var24 = var22->m_blocks[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + var1];
						int var17 = Block::s_lightOpacity[var24];
						if(var17 > 100) {
							var16 = 0;
						} else if(var16 < 15) {
							var17 = var17;
							if(var17 == 0) {
								var17 = 1;
							}

							int var18;
							if(var1 > 0) {
								var18 = (var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + (var1 - 1)] & 15) - var17;
								if(var18 > var16) {
									var16 = var18;
								}
							}

							if(var1 < var22->m_worldWidth - 1) {
								var18 = (var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + var1 + 1] & 15) - var17;
								if(var18 > var16) {
									var16 = var18;
								}
							}

							if(var14 > 0) {
								var18 = (var22->m_floatValue[((var14 - 1) * var22->m_worldLength + var15) * var22->m_worldWidth + var1] & 15) - var17;
								if(var18 > var16) {
									var16 = var18;
								}
							}

							if(var14 < var22->m_worldHeight - 1) {
								var18 = (var22->m_floatValue[((var14 + 1) * var22->m_worldLength + var15) * var22->m_worldWidth + var1] & 15) - var17;
								if(var18 > var16) {
									var16 = var18;
								}
							}

							if(var15 > 0) {
								var18 = (var22->m_floatValue[(var14 * var22->m_worldLength + (var15 - 1)) * var22->m_worldWidth + var1] & 15) - var17;
								if(var18 > var16) {
									var16 = var18;
								}
							}

							if(var15 < var22->m_worldLength - 1) {
								var18 = (var22->m_floatValue[(var14 * var22->m_worldLength + var15 + 1) * var22->m_worldWidth + var1] & 15) - var17;
								if(var18 > var16) {
									var16 = var18;
								}
							}
						}

						if(var16 < Block::s_lightValue[var24]) {
							var16 = Block::s_lightValue[var24];
						}

						var17 = var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + var1] & 15;
						if(var17 != var16) {
							var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + var1] = (unsigned char)((var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + var1] & 240) + var16);
							if(var1 > 0 && (var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + (var1 - 1)] & 15) != var16 - 1) {
								var13 = var1 - 1 + var14 * var22->m_worldWidth + var15 * var22->m_worldWidth * var22->m_worldHeight;
								if((var22->m_chunks[var13 >> 3] & 1 << (var13 & 7)) == 0) {
									var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] | 1 << (var13 & 7));
									var22->m_lightingUpdateList3[var22->m_lightingUpdateCounter++] = var13;
								}
							}

							if(var1 < var22->m_worldWidth - 1 && (var22->m_floatValue[(var14 * var22->m_worldLength + var15) * var22->m_worldWidth + var1 + 1] & 15) != var16 - 1) {
								var13 = var1 + 1 + var14 * var22->m_worldWidth + var15 * var22->m_worldWidth * var22->m_worldHeight;
								if((var22->m_chunks[var13 >> 3] & 1 << (var13 & 7)) == 0) {
									var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] | 1 << (var13 & 7));
									var22->m_lightingUpdateList3[var22->m_lightingUpdateCounter++] = var13;
								}
							}

							if(var14 > 0 && (var22->m_floatValue[((var14 - 1) * var22->m_worldLength + var15) * var22->m_worldWidth + var1] & 15) != var16 - 1) {
								var13 = var1 + (var14 - 1) * var22->m_worldWidth + var15 * var22->m_worldWidth * var22->m_worldHeight;
								if((var22->m_chunks[var13 >> 3] & 1 << (var13 & 7)) == 0) {
									var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] | 1 << (var13 & 7));
									var22->m_lightingUpdateList3[var22->m_lightingUpdateCounter++] = var13;
								}
							}

							if(var14 < var22->m_worldHeight - 1 && (var22->m_floatValue[((var14 + 1) * var22->m_worldLength + var15) * var22->m_worldWidth + var1] & 15) != var16 - 1) {
								var13 = var1 + (var14 + 1) * var22->m_worldWidth + var15 * var22->m_worldWidth * var22->m_worldHeight;
								if((var22->m_chunks[var13 >> 3] & 1 << (var13 & 7)) == 0) {
									var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] | 1 << (var13 & 7));
									var22->m_lightingUpdateList3[var22->m_lightingUpdateCounter++] = var13;
								}
							}

							if(var15 > 0 && (var22->m_floatValue[(var14 * var22->m_worldLength + (var15 - 1)) * var22->m_worldWidth + var1] & 15) != var16 - 1) {
								var13 = var1 + var14 * var22->m_worldWidth + (var15 - 1) * var22->m_worldWidth * var22->m_worldHeight;
								if((var22->m_chunks[var13 >> 3] & 1 << (var13 & 7)) == 0) {
									var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] | 1 << (var13 & 7));
									var22->m_lightingUpdateList3[var22->m_lightingUpdateCounter++] = var13;
								}
							}

							if(var15 < var22->m_worldLength - 1 && (var22->m_floatValue[(var14 * var22->m_worldLength + var15 + 1) * var22->m_worldWidth + var1] & 15) != var16 - 1) {
								var13 = var1 + var14 * var22->m_worldWidth + (var15 + 1) * var22->m_worldWidth * var22->m_worldHeight;
								if((var22->m_chunks[var13 >> 3] & 1 << (var13 & 7)) == 0) {
									var22->m_chunks[var13 >> 3] = (unsigned char)(var22->m_chunks[var13 >> 3] | 1 << (var13 & 7));
									var22->m_lightingUpdateList3[var22->m_lightingUpdateCounter++] = var13;
								}
							}

							if(var5 == -999) {
								var5 = var1;
								var6 = var1;
								var7 = var14;
								var8 = var14;
								var9 = var15;
								var10 = var15;
							}

							if(var1 < var5) {
								var5 = var1;
							} else if(var1 > var6) {
								var6 = var1;
							}

							if(var14 > var8) {
								var8 = var14;
							} else if(var14 < var7) {
								var7 = var14;
							}

							if(var15 < var9) {
								var9 = var15;
							} else if(var15 > var10) {
								var10 = var15;
							}
						}
					}
				}

				if(var5 > -999) {
					var22->m_skyLightList.push_back(MetadataChunkBlock(var22, var5, var7, var9, var6, var8, var10));
				}

				if(var12 > 0) {
					var20 = true;
				}
			}

		}
}

const std::string& Light::debugSkylightUpdates() {
		return "" + (m_blockLightList.size() + m_skyLightList.size());
}
