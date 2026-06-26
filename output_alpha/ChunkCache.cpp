#include "stdafx.h"
// Auto-generated from ChunkCache.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE:
//  - m_field_1062_c is Chunk*** (Chunk[][]); allocate rows*cols of Chunk* and index twice.
//  - cell access uses -> (each cell is a Chunk*), was '.'.

#include "ChunkCache.h"
#include "Common.h"
#include "Chunk.h"
#include "Material.h"
#include "TileEntity.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "Block.h"
#include "WorldProvider.h"

ChunkCache::ChunkCache()
	: m_field_1060_a(0)
	, m_field_1059_b(0)
	, m_field_1062_c(NULL)
	, worldObj(NULL)
	, m_field_1062_c_count(0)
	, m_field_1062_c_inner(0)
{
}

ChunkCache::ChunkCache(World* var1, int var2, int var3, int var4, int var5, int var6, int var7)
	: m_field_1060_a(0)
	, m_field_1059_b(0)
	, m_field_1062_c(NULL)
	, worldObj(NULL)
	, m_field_1062_c_count(0)
	, m_field_1062_c_inner(0)
{
		worldObj = var1;
		m_field_1060_a = var2 >> 4;
		m_field_1059_b = var4 >> 4;
		int var8 = var5 >> 4;
		int var9 = var7 >> 4;
		// Java: chunkArray = new Chunk[var8 - chunkX + 1][var9 - chunkZ + 1];
		int rows = var8 - m_field_1060_a + 1;
		int cols = var9 - m_field_1059_b + 1;
		m_field_1062_c_count = rows;
		m_field_1062_c_inner = cols;
		m_field_1062_c = new Chunk**[rows];
		for(int _r = 0; _r < rows; ++_r) {
			m_field_1062_c[_r] = new Chunk*[cols];
			for(int _c = 0; _c < cols; ++_c) {
				m_field_1062_c[_r][_c] = NULL;
			}
		}

		for(int var10 = m_field_1060_a; var10 <= var8; ++var10) {
			for(int var11 = m_field_1059_b; var11 <= var9; ++var11) {
				m_field_1062_c[var10 - m_field_1060_a][var11 - m_field_1059_b] = var1->getChunkFromChunkCoords(var10, var11);
			}
		}

}

ChunkCache::~ChunkCache() {
	if(m_field_1062_c != NULL) {
		for(int _r = 0; _r < m_field_1062_c_count; ++_r) {
			delete[] m_field_1062_c[_r];
		}
		delete[] m_field_1062_c;
		m_field_1062_c = NULL;
	}
	worldObj = NULL;
}

int ChunkCache::getBlockId(int var1, int var2, int var3) {
		if(var2 < 0) {
			return 0;
		} else if(var2 >= 128) {
			return 0;
		} else {
			int var4 = (var1 >> 4) - m_field_1060_a;
			int var5 = (var3 >> 4) - m_field_1059_b;
			return m_field_1062_c[var4][var5]->getBlockID(var1 & 15, var2, var3 & 15);
		}
}

TileEntity* ChunkCache::getBlockTileEntity(int var1, int var2, int var3) {
		int var4 = (var1 >> 4) - m_field_1060_a;
		int var5 = (var3 >> 4) - m_field_1059_b;
		return m_field_1062_c[var4][var5]->getChunkBlockTileEntity(var1 & 15, var2, var3 & 15);
}

float ChunkCache::getLightBrightness(int var1, int var2, int var3) {
		return worldObj->m_worldProvider->m_lightBrightnessTable[func_4086_d(var1, var2, var3)];
}

int ChunkCache::func_4086_d(int var1, int var2, int var3) {
		return func_716_a(var1, var2, var3, true);
}

int ChunkCache::func_716_a(int var1, int var2, int var3, bool var4) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			int var5;
			int var6;
			if(var4) {
				var5 = getBlockId(var1, var2, var3);
				if(var5 == Block::s_stairSingle->blockID || var5 == Block::s_tilledField->blockID) {
					var6 = func_716_a(var1, var2 + 1, var3, false);
					int var7 = func_716_a(var1 + 1, var2, var3, false);
					int var8 = func_716_a(var1 - 1, var2, var3, false);
					int var9 = func_716_a(var1, var2, var3 + 1, false);
					int var10 = func_716_a(var1, var2, var3 - 1, false);
					if(var7 > var6) {
						var6 = var7;
					}

					if(var8 > var6) {
						var6 = var8;
					}

					if(var9 > var6) {
						var6 = var9;
					}

					if(var10 > var6) {
						var6 = var10;
					}

					return var6;
				}
			}

			if(var2 < 0) {
				return 0;
			} else if(var2 >= 128) {
				var5 = 15 - worldObj->m_skylightSubtracted;
				if(var5 < 0) {
					var5 = 0;
				}

				return var5;
			} else {
				var5 = (var1 >> 4) - m_field_1060_a;
				var6 = (var3 >> 4) - m_field_1059_b;
				return m_field_1062_c[var5][var6]->getBlockLightValue(var1 & 15, var2, var3 & 15, worldObj->m_skylightSubtracted);
			}
		} else {
			return 15;
		}
}

int ChunkCache::getBlockMetadata(int var1, int var2, int var3) {
		if(var2 < 0) {
			return 0;
		} else if(var2 >= 128) {
			return 0;
		} else {
			int var4 = (var1 >> 4) - m_field_1060_a;
			int var5 = (var3 >> 4) - m_field_1059_b;
			return m_field_1062_c[var4][var5]->getBlockMetadata(var1 & 15, var2, var3 & 15);
		}
}

Material* ChunkCache::getBlockMaterial(int var1, int var2, int var3) {
		int var4 = getBlockId(var1, var2, var3);
		return var4 == 0 ? Material::s_air : Block::blocksList[var4]->m_blockMaterial;
}

bool ChunkCache::isBlockOpaqueCube(int var1, int var2, int var3) {
		Block* var4 = Block::blocksList[getBlockId(var1, var2, var3)];
		return var4 == NULL ? false : var4->isOpaqueCube();
}

WorldChunkManager* ChunkCache::func_4075_a() {
		return worldObj->func_4075_a();
}
