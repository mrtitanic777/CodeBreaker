#include "stdafx.h"
// Hand-ported from Chunk.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
// Core logic only (no GL). 16x128x16 block/light/metadata storage.

#include "Chunk.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EnumSkyBlock.h"
#include "NibbleArray.h"
#include "Random.h"
#include "TileEntity.h"
#include "World.h"
#include "WorldProvider.h"
#include "Block.h"
#include "BlockContainer.h"
#include "ChunkPosition.h"

#include <math.h>
#include <algorithm>

// ---------------------------------------------------------------------------
// Java's chunkTileEntityMap is keyed by ChunkPosition (value-equality on x/y/z).
// The converted header types it as std::map<std::string,void*>, so we encode the
// ChunkPosition into a stable string key. (PORT: was a HashMap<ChunkPosition,..>)
// ---------------------------------------------------------------------------
static std::string posKey(int x, int y, int z) {
	char buf[48];
	sprintf(buf, "%d,%d,%d", x, y, z);
	return std::string(buf);
}

// Number of vertical entity sub-lists (Java: entities = new List[8]).
static const int ENTITY_SECTIONS = 8;
// Blocks per chunk: 16 * 16 * 128.
static const int CHUNK_BLOCK_COUNT = 32768;

bool Chunk::s_field_1540_a;

Chunk::Chunk()
	: m_blocks(0)
	, m_isChunkLoaded(false)
	, worldObj(NULL)
	, m_data(NULL)
	, m_skylightMap(NULL)
	, m_blocklightMap(NULL)
	, m_heightMap(0)
	, m_field_1532_i(0)
	, m_xPosition(0)
	, m_zPosition(0)
	, m_chunkTileEntityMap(NULL)
	, m_entities(NULL)
	, m_isTerrainPopulated(false)
	, m_isModified(false)
	, m_neverSave(false)
	, m_field_1524_q(false)
	, m_hasEntities(false)
	, m_lastSaveTime(0)
	, m_blocks_count(0)
	, m_heightMap_count(0)
	, m_entities_count(0)
{
}

Chunk::Chunk(World* var1, int var2, int var3)
	: m_blocks(0)
	, m_isChunkLoaded(false)
	, worldObj(NULL)
	, m_data(NULL)
	, m_skylightMap(NULL)
	, m_blocklightMap(NULL)
	, m_heightMap(0)
	, m_field_1532_i(0)
	, m_xPosition(0)
	, m_zPosition(0)
	, m_chunkTileEntityMap(NULL)
	, m_entities(NULL)
	, m_isTerrainPopulated(false)
	, m_isModified(false)
	, m_neverSave(false)
	, m_field_1524_q(false)
	, m_hasEntities(false)
	, m_lastSaveTime(0)
	, m_blocks_count(0)
	, m_heightMap_count(0)
	, m_entities_count(0)
{
	m_chunkTileEntityMap = new std::map<std::string, void*>();
	m_entities = new std::vector<void*>[ENTITY_SECTIONS];
	m_entities_count = ENTITY_SECTIONS;
	m_isTerrainPopulated = false;
	m_isModified = false;
	m_field_1524_q = false;
	m_hasEntities = false;
	m_lastSaveTime = 0L;
	worldObj = var1;
	m_xPosition = var2;
	m_zPosition = var3;
	m_heightMap = new unsigned char[256];
	m_heightMap_count = 256;

	for(int var4 = 0; var4 < m_entities_count; ++var4) {
		m_entities[var4].clear();
	}
}

Chunk::Chunk(World* var1, unsigned char* var2, int var3, int var4)
	: m_blocks(0)
	, m_isChunkLoaded(false)
	, worldObj(NULL)
	, m_data(NULL)
	, m_skylightMap(NULL)
	, m_blocklightMap(NULL)
	, m_heightMap(0)
	, m_field_1532_i(0)
	, m_xPosition(0)
	, m_zPosition(0)
	, m_chunkTileEntityMap(NULL)
	, m_entities(NULL)
	, m_isTerrainPopulated(false)
	, m_isModified(false)
	, m_neverSave(false)
	, m_field_1524_q(false)
	, m_hasEntities(false)
	, m_lastSaveTime(0)
	, m_blocks_count(0)
	, m_heightMap_count(0)
	, m_entities_count(0)
{
	// Java: this(var1, var3, var4); -- replicate the (World,int,int) ctor body.
	m_chunkTileEntityMap = new std::map<std::string, void*>();
	m_entities = new std::vector<void*>[ENTITY_SECTIONS];
	m_entities_count = ENTITY_SECTIONS;
	worldObj = var1;
	m_xPosition = var3;
	m_zPosition = var4;
	m_heightMap = new unsigned char[256];
	m_heightMap_count = 256;
	for(int var5 = 0; var5 < m_entities_count; ++var5) {
		m_entities[var5].clear();
	}

	// Java: this.blocks = var2; data/skylight/blocklight = new NibbleArray(var2.length)
	m_blocks = var2;
	m_blocks_count = CHUNK_BLOCK_COUNT;
	m_data = new NibbleArray(CHUNK_BLOCK_COUNT);
	m_skylightMap = new NibbleArray(CHUNK_BLOCK_COUNT);
	m_blocklightMap = new NibbleArray(CHUNK_BLOCK_COUNT);
}

Chunk::~Chunk() {
	delete[] m_blocks;
	m_blocks = NULL;
	worldObj = NULL;
	delete m_data;
	m_data = NULL;
	delete m_skylightMap;
	m_skylightMap = NULL;
	delete m_blocklightMap;
	m_blocklightMap = NULL;
	delete[] m_heightMap;
	m_heightMap = NULL;
	delete m_chunkTileEntityMap;
	m_chunkTileEntityMap = NULL;
	delete[] m_entities;
	m_entities = NULL;
}

bool Chunk::isAtLocation(int var1, int var2) {
	return var1 == m_xPosition && var2 == m_zPosition;
}

int Chunk::getHeightValue(int var1, int var2) {
	return m_heightMap[var2 << 4 | var1] & 255;
}

void Chunk::func_1014_a() {
	// Java: empty body.
}

void Chunk::generateHeightMap() {
	int var1 = 127;

	for(int var2 = 0; var2 < 16; ++var2) {
		for(int var3 = 0; var3 < 16; ++var3) {
			int var4 = 127;

			for(int var5 = var2 << 11 | var3 << 7; var4 > 0 && Block::s_lightOpacity[m_blocks[var5 + var4 - 1]] == 0; --var4) {
			}

			m_heightMap[var3 << 4 | var2] = (unsigned char)var4;
			if(var4 < var1) {
				var1 = var4;
			}
		}
	}

	m_field_1532_i = var1;
	m_isModified = true;
}

void Chunk::func_1024_c() {
	int var1 = 127;

	int var2;
	int var3;
	for(var2 = 0; var2 < 16; ++var2) {
		for(var3 = 0; var3 < 16; ++var3) {
			m_heightMap[var3 << 4 | var2] = (unsigned char)(-128);
			func_1003_g(var2, 127, var3);
			if((m_heightMap[var3 << 4 | var2] & 255) < var1) {
				var1 = m_heightMap[var3 << 4 | var2] & 255;
			}
		}
	}

	m_field_1532_i = var1;

	for(var2 = 0; var2 < 16; ++var2) {
		for(var3 = 0; var3 < 16; ++var3) {
			func_996_c(var2, var3);
		}
	}

	m_isModified = true;
}

void Chunk::func_4143_d() {
	int var1 = 32;

	for(int var2 = 0; var2 < 16; ++var2) {
		for(int var3 = 0; var3 < 16; ++var3) {
			int var4 = var2 << 11 | var3 << 7;

			int var5;
			int var6;
			for(var5 = 0; var5 < 128; ++var5) {
				var6 = Block::s_lightValue[m_blocks[var4 + var5]];
				if(var6 > 0) {
					m_blocklightMap->setNibble(var2, var5, var3, var6);
				}
			}

			var5 = 15;

			for(var6 = var1 - 2; var6 < 128 && var5 > 0; m_blocklightMap->setNibble(var2, var6, var3, var5)) {
				++var6;
				unsigned char var7 = m_blocks[var4 + var6];
				int var8 = Block::s_lightOpacity[var7];
				int var9 = Block::s_lightValue[var7];
				if(var8 == 0) {
					var8 = 1;
				}

				var5 -= var8;
				if(var9 > var5) {
					var5 = var9;
				}

				if(var5 < 0) {
					var5 = 0;
				}
			}
		}
	}

	worldObj->func_616_a(EnumSkyBlock::s_Block, m_xPosition * 16, var1 - 1, m_zPosition * 16, m_xPosition * 16 + 16, var1 + 1, m_zPosition * 16 + 16);
	m_isModified = true;
}

void Chunk::func_996_c(int var1, int var2) {
	int var3 = getHeightValue(var1, var2);
	int var4 = m_xPosition * 16 + var1;
	int var5 = m_zPosition * 16 + var2;
	func_1020_f(var4 - 1, var5, var3);
	func_1020_f(var4 + 1, var5, var3);
	func_1020_f(var4, var5 - 1, var3);
	func_1020_f(var4, var5 + 1, var3);
}

void Chunk::func_1020_f(int var1, int var2, int var3) {
	int var4 = worldObj->getHeightValue(var1, var2);
	if(var4 > var3) {
		worldObj->func_616_a(EnumSkyBlock::s_Sky, var1, var3, var2, var1, var4, var2);
	} else if(var4 < var3) {
		worldObj->func_616_a(EnumSkyBlock::s_Sky, var1, var4, var2, var1, var3, var2);
	}

	m_isModified = true;
}

void Chunk::func_1003_g(int var1, int var2, int var3) {
	int var4 = m_heightMap[var3 << 4 | var1] & 255;
	int var5 = var4;
	if(var2 > var4) {
		var5 = var2;
	}

	for(int var6 = var1 << 11 | var3 << 7; var5 > 0 && Block::s_lightOpacity[m_blocks[var6 + var5 - 1]] == 0; --var5) {
	}

	if(var5 != var4) {
		worldObj->func_680_f(var1, var3, var5, var4);
		m_heightMap[var3 << 4 | var1] = (unsigned char)var5;
		int var7;
		int var8;
		int var9;
		if(var5 < m_field_1532_i) {
			m_field_1532_i = var5;
		} else {
			var7 = 127;

			for(var8 = 0; var8 < 16; ++var8) {
				for(var9 = 0; var9 < 16; ++var9) {
					if((m_heightMap[var9 << 4 | var8] & 255) < var7) {
						var7 = m_heightMap[var9 << 4 | var8] & 255;
					}
				}
			}

			m_field_1532_i = var7;
		}

		var7 = m_xPosition * 16 + var1;
		var8 = m_zPosition * 16 + var3;
		if(var5 < var4) {
			for(var9 = var5; var9 < var4; ++var9) {
				m_skylightMap->setNibble(var1, var9, var3, 15);
			}
		} else {
			worldObj->func_616_a(EnumSkyBlock::s_Sky, var7, var4, var8, var7, var5, var8);

			for(var9 = var4; var9 < var5; ++var9) {
				m_skylightMap->setNibble(var1, var9, var3, 0);
			}
		}

		var9 = 15;

		int var10;
		for(var10 = var5; var5 > 0 && var9 > 0; m_skylightMap->setNibble(var1, var5, var3, var9)) {
			--var5;
			int var11 = Block::s_lightOpacity[getBlockID(var1, var5, var3)];
			if(var11 == 0) {
				var11 = 1;
			}

			var9 -= var11;
			if(var9 < 0) {
				var9 = 0;
			}
		}

		while(var5 > 0 && Block::s_lightOpacity[getBlockID(var1, var5 - 1, var3)] == 0) {
			--var5;
		}

		if(var5 != var10) {
			worldObj->func_616_a(EnumSkyBlock::s_Sky, var7 - 1, var5, var8 - 1, var7 + 1, var10, var8 + 1);
		}

		m_isModified = true;
	}
}

int Chunk::getBlockID(int var1, int var2, int var3) {
	return m_blocks[var1 << 11 | var3 << 7 | var2];
}

bool Chunk::setBlockIDWithMetadata(int var1, int var2, int var3, int var4, int var5) {
	unsigned char var6 = (unsigned char)var4;
	int var7 = m_heightMap[var3 << 4 | var1] & 255;
	int var8 = m_blocks[var1 << 11 | var3 << 7 | var2] & 255;
	if(var8 == var4 && m_data->getNibble(var1, var2, var3) == var5) {
		return false;
	} else {
		int var9 = m_xPosition * 16 + var1;
		int var10 = m_zPosition * 16 + var3;
		m_blocks[var1 << 11 | var3 << 7 | var2] = var6;
		if(var8 != 0 && !worldObj->m_multiplayerWorld) {
			Block::blocksList[var8]->onBlockRemoval(worldObj, var9, var2, var10);
		}

		m_data->setNibble(var1, var2, var3, var5);
		if(!worldObj->m_worldProvider->m_field_6478_e) {
			if(Block::s_lightOpacity[var6] != 0) {
				if(var2 >= var7) {
					func_1003_g(var1, var2 + 1, var3);
				}
			} else if(var2 == var7 - 1) {
				func_1003_g(var1, var2, var3);
			}

			worldObj->func_616_a(EnumSkyBlock::s_Sky, var9, var2, var10, var9, var2, var10);
		}

		worldObj->func_616_a(EnumSkyBlock::s_Block, var9, var2, var10, var9, var2, var10);
		func_996_c(var1, var3);
		if(var4 != 0) {
			Block::blocksList[var4]->onBlockAdded(worldObj, var9, var2, var10);
		}

		m_data->setNibble(var1, var2, var3, var5);
		m_isModified = true;
		return true;
	}
}

bool Chunk::setBlockID(int var1, int var2, int var3, int var4) {
	unsigned char var5 = (unsigned char)var4;
	int var6 = m_heightMap[var3 << 4 | var1] & 255;
	int var7 = m_blocks[var1 << 11 | var3 << 7 | var2] & 255;
	if(var7 == var4) {
		return false;
	} else {
		int var8 = m_xPosition * 16 + var1;
		int var9 = m_zPosition * 16 + var3;
		m_blocks[var1 << 11 | var3 << 7 | var2] = var5;
		if(var7 != 0) {
			Block::blocksList[var7]->onBlockRemoval(worldObj, var8, var2, var9);
		}

		m_data->setNibble(var1, var2, var3, 0);
		if(Block::s_lightOpacity[var5] != 0) {
			if(var2 >= var6) {
				func_1003_g(var1, var2 + 1, var3);
			}
		} else if(var2 == var6 - 1) {
			func_1003_g(var1, var2, var3);
		}

		worldObj->func_616_a(EnumSkyBlock::s_Sky, var8, var2, var9, var8, var2, var9);
		worldObj->func_616_a(EnumSkyBlock::s_Block, var8, var2, var9, var8, var2, var9);
		func_996_c(var1, var3);
		if(var4 != 0 && !worldObj->m_multiplayerWorld) {
			Block::blocksList[var4]->onBlockAdded(worldObj, var8, var2, var9);
		}

		m_isModified = true;
		return true;
	}
}

int Chunk::getBlockMetadata(int var1, int var2, int var3) {
	return m_data->getNibble(var1, var2, var3);
}

void Chunk::setBlockMetadata(int var1, int var2, int var3, int var4) {
	m_isModified = true;
	m_data->setNibble(var1, var2, var3, var4);
}

int Chunk::getSavedLightValue(EnumSkyBlock* var1, int var2, int var3, int var4) {
	return var1 == EnumSkyBlock::s_Sky ? m_skylightMap->getNibble(var2, var3, var4) : (var1 == EnumSkyBlock::s_Block ? m_blocklightMap->getNibble(var2, var3, var4) : 0);
}

void Chunk::setLightValue(EnumSkyBlock* var1, int var2, int var3, int var4, int var5) {
	m_isModified = true;
	if(var1 == EnumSkyBlock::s_Sky) {
		m_skylightMap->setNibble(var2, var3, var4, var5);
	} else {
		if(var1 != EnumSkyBlock::s_Block) {
			return;
		}

		m_blocklightMap->setNibble(var2, var3, var4, var5);
	}
}

int Chunk::getBlockLightValue(int var1, int var2, int var3, int var4) {
	int var5 = m_skylightMap->getNibble(var1, var2, var3);
	if(var5 > 0) {
		s_field_1540_a = true;
	}

	var5 -= var4;
	int var6 = m_blocklightMap->getNibble(var1, var2, var3);
	if(var6 > var5) {
		var5 = var6;
	}

	return var5;
}

void Chunk::addEntity(Entity* var1) {
	if(!m_field_1524_q) {
		m_hasEntities = true;
		int var2 = (int)floor(var1->posX / 16.0);
		int var3 = (int)floor(var1->posZ / 16.0);
		if(var2 != m_xPosition || var3 != m_zPosition) {
			DbgPrintF("Wrong location! entity at chunk %d,%d\n", var2, var3);
			// PORT: Thread.dumpStack() -- no stack-dump on 360.
		}

		int var4 = (int)floor(var1->posY / 16.0);
		if(var4 < 0) {
			var4 = 0;
		}

		if(var4 >= m_entities_count) {
			var4 = m_entities_count - 1;
		}

		var1->m_field_621_aZ = true;
		var1->m_field_657_ba = m_xPosition;
		var1->m_field_656_bb = var4;
		var1->m_field_654_bc = m_zPosition;
		m_entities[var4].push_back(var1);
	}
}

void Chunk::func_1015_b(Entity* var1) {
	func_1016_a(var1, var1->m_field_656_bb);
}

void Chunk::func_1016_a(Entity* var1, int var2) {
	if(var2 < 0) {
		var2 = 0;
	}

	if(var2 >= m_entities_count) {
		var2 = m_entities_count - 1;
	}

	_vecRemoveObj(m_entities[var2], (void*)var1);
}

bool Chunk::canBlockSeeTheSky(int var1, int var2, int var3) {
	return var2 >= (m_heightMap[var3 << 4 | var1] & 255);
}

TileEntity* Chunk::getChunkBlockTileEntity(int var1, int var2, int var3) {
	std::string var4 = posKey(var1, var2, var3);
	TileEntity* var5 = NULL;
	std::map<std::string, void*>::iterator it = m_chunkTileEntityMap->find(var4);
	if(it != m_chunkTileEntityMap->end()) {
		var5 = (TileEntity*)it->second;
	}

	if(var5 == NULL) {
		int var6 = getBlockID(var1, var2, var3);
		if(!Block::s_isBlockContainer[var6]) {
			return NULL;
		}

		BlockContainer* var7 = (BlockContainer*)Block::blocksList[var6];
		var7->onBlockAdded(worldObj, m_xPosition * 16 + var1, var2, m_zPosition * 16 + var3);
		it = m_chunkTileEntityMap->find(var4);
		var5 = (it != m_chunkTileEntityMap->end()) ? (TileEntity*)it->second : NULL;
	}

	return var5;
}

void Chunk::func_1001_a(TileEntity* var1) {
	int var2 = var1->xCoord - m_xPosition * 16;
	int var3 = var1->yCoord;
	int var4 = var1->zCoord - m_zPosition * 16;
	setChunkBlockTileEntity(var2, var3, var4, var1);
}

void Chunk::setChunkBlockTileEntity(int var1, int var2, int var3, TileEntity* var4) {
	std::string var5 = posKey(var1, var2, var3);
	var4->worldObj = worldObj;
	var4->xCoord = m_xPosition * 16 + var1;
	var4->yCoord = var2;
	var4->zCoord = m_zPosition * 16 + var3;

	int var6 = getBlockID(var1, var2, var3);
	if(var6 != 0 && Block::s_isBlockContainer[var6]) {
		if(m_isChunkLoaded) {
			std::map<std::string, void*>::iterator it = m_chunkTileEntityMap->find(var5);
			if(it != m_chunkTileEntityMap->end() && it->second != NULL) {
				_vecRemoveObj(worldObj->m_loadedTileEntityList, it->second);
			}

			worldObj->m_loadedTileEntityList.push_back((void*)var4);
		}

		(*m_chunkTileEntityMap)[var5] = (void*)var4;
	} else {
		DbgPrintF("%s\n", "Attempted to place a tile entity where there was no entity tile!");
	}
}

void Chunk::removeChunkBlockTileEntity(int var1, int var2, int var3) {
	std::string var4 = posKey(var1, var2, var3);
	std::map<std::string, void*>::iterator it = m_chunkTileEntityMap->find(var4);
	void* removed = (it != m_chunkTileEntityMap->end()) ? it->second : NULL;
	if(it != m_chunkTileEntityMap->end()) {
		m_chunkTileEntityMap->erase(it);
	}

	if(m_isChunkLoaded && removed != NULL) {
		_vecRemoveObj(worldObj->m_loadedTileEntityList, removed);
	}
}

void Chunk::onChunkLoad() {
	m_isChunkLoaded = true;

	// Java: worldObj.loadedTileEntityList.addAll(chunkTileEntityMap.values());
	for(std::map<std::string, void*>::iterator it = m_chunkTileEntityMap->begin(); it != m_chunkTileEntityMap->end(); ++it) {
		worldObj->m_loadedTileEntityList.push_back(it->second);
	}

	for(int var1 = 0; var1 < m_entities_count; ++var1) {
		worldObj->func_636_a(m_entities[var1]);
	}
}

void Chunk::onChunkUnload() {
	m_isChunkLoaded = false;

	// Java: worldObj.loadedTileEntityList.removeAll(chunkTileEntityMap.values());
	for(std::map<std::string, void*>::iterator it = m_chunkTileEntityMap->begin(); it != m_chunkTileEntityMap->end(); ++it) {
		_vecRemoveObj(worldObj->m_loadedTileEntityList, it->second);
	}

	for(int var1 = 0; var1 < m_entities_count; ++var1) {
		worldObj->func_632_b(m_entities[var1]);
	}
}

void Chunk::setChunkModified() {
	m_isModified = true;
}

void Chunk::getEntitiesWithinAABBForEntity(Entity* var1, AxisAlignedBB var2, std::vector<void*> var3) {
	int var4 = (int)floor((var2.minY - 2.0) / 16.0);
	int var5 = (int)floor((var2.maxY + 2.0) / 16.0);
	if(var4 < 0) {
		var4 = 0;
	}

	if(var5 >= m_entities_count) {
		var5 = m_entities_count - 1;
	}

	for(int var6 = var4; var6 <= var5; ++var6) {
		std::vector<void*>& var7 = m_entities[var6];

		for(size_t var8 = 0; var8 < var7.size(); ++var8) {
			Entity* var9 = (Entity*)var7[var8];
			if(var9 != var1 && var9->boundingBox.intersectsWith(var2)) {
				var3.push_back(var9);
			}
		}
	}
}

void Chunk::getEntitiesOfTypeWithinAAAB(void* var1, AxisAlignedBB var2, std::vector<void*> var3) {
	int var4 = (int)floor((var2.minY - 2.0) / 16.0);
	int var5 = (int)floor((var2.maxY + 2.0) / 16.0);
	if(var4 < 0) {
		var4 = 0;
	}

	if(var5 >= m_entities_count) {
		var5 = m_entities_count - 1;
	}

	for(int var6 = var4; var6 <= var5; ++var6) {
		std::vector<void*>& var7 = m_entities[var6];

		for(size_t var8 = 0; var8 < var7.size(); ++var8) {
			Entity* var9 = (Entity*)var7[var8];
			// PORT: Java did var1(Class).isAssignableFrom(var9.getClass()) -- no RTTI
			// class-token here; accept all entities (caller filters as needed).
			if(var9->boundingBox.intersectsWith(var2)) {
				var3.push_back(var9);
			}
		}
	}
}

bool Chunk::needsSaving(bool var1) {
	return m_neverSave ? false : (m_hasEntities && worldObj->m_worldTime != m_lastSaveTime ? true : m_isModified);
}

int Chunk::func_1004_a(unsigned char* var1, int var2, int var3, int var4, int var5, int var6, int var7, int var8) {
	int var9;
	int var10;
	int var11;
	int var12;
	for(var9 = var2; var9 < var5; ++var9) {
		for(var10 = var4; var10 < var7; ++var10) {
			var11 = var9 << 11 | var10 << 7 | var3;
			var12 = var6 - var3;
			memcpy(m_blocks + var11, var1 + var8, var12);
			var8 += var12;
		}
	}

	generateHeightMap();

	for(var9 = var2; var9 < var5; ++var9) {
		for(var10 = var4; var10 < var7; ++var10) {
			var11 = (var9 << 11 | var10 << 7 | var3) >> 1;
			var12 = (var6 - var3) / 2;
			memcpy(m_data->m_data + var11, var1 + var8, var12);
			var8 += var12;
		}
	}

	for(var9 = var2; var9 < var5; ++var9) {
		for(var10 = var4; var10 < var7; ++var10) {
			var11 = (var9 << 11 | var10 << 7 | var3) >> 1;
			var12 = (var6 - var3) / 2;
			memcpy(m_blocklightMap->m_data + var11, var1 + var8, var12);
			var8 += var12;
		}
	}

	for(var9 = var2; var9 < var5; ++var9) {
		for(var10 = var4; var10 < var7; ++var10) {
			var11 = (var9 << 11 | var10 << 7 | var3) >> 1;
			var12 = (var6 - var3) / 2;
			memcpy(m_skylightMap->m_data + var11, var1 + var8, var12);
			var8 += var12;
		}
	}

	return var8;
}

Random* Chunk::func_997_a(long long var1) {
	long long seed = worldObj->m_randomSeed
		+ (long long)(m_xPosition * m_xPosition * 4987142)
		+ (long long)(m_xPosition * 5947611)
		+ (long long)(m_zPosition * m_zPosition) * 4392871LL
		+ (long long)(m_zPosition * 389711);
	seed ^= var1;
	return new Random((unsigned long long)seed);
}
