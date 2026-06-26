#include "stdafx.h"
// Auto-generated from World.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "World.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Class.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityMap.h"
#include "IWorldAccess.h"
#include "Light.h"
#include "Map.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "Pathfinder.h"
#include "Random.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include <algorithm>
#include <string>
#include "Block.h"
#include "Vec3.h"

float* World::s_lightBrightnessTable = new float[16];
short World::s_floodFillCounter;

World::World()
	: width(0)
	, m_length(0)
	, height(0)
	, m_d(0)
	, m_e(0)
	, m_createTime(0)
	, m_xSpawn(0)
	, m_ySpawn(0)
	, m_zSpawn(0)
	, m_rotSpawn(0)
	, m_defaultFluid(Block::s_waterMoving->blockID)
	, m_map(NULL)
	, m_heightMap(0)
	, m_random(new Random())
	, rand(new Random())
	, m_randId(random->nextInt())
	, m_entityMap(NULL)
	, m_waterLevel(0)
	, m_groundLevel(0)
	, m_cloudHeight(0)
	, m_skyColor(10079487)
	, m_fogColor(16777215)
	, m_cloudColor(16777215)
	, m_updateLCG(0)
	, m_playTime(0)
	, m_playerEntity(NULL)
	, m_survivalWorld(true)
	, m_skyBrightness(15)
	, m_skylightSubtracted(15)
	, m_pathFinder(new Pathfinder(this))
	, m_L(NULL)
	, m_worldTime(0)
	, m_floodFillCounters(new short[1048576])
	, m_coords(new int[1048576])
	, m_floodedBlocks(new int[1048576])
	, m_difficultySetting(2)
{
}

World::~World() {
	delete[] m_d;
	m_d = NULL;
	delete[] m_e;
	m_e = NULL;
	m_map = NULL;
	delete[] m_heightMap;
	m_heightMap = NULL;
	m_random = NULL;
	rand = NULL;
	m_entityMap = NULL;
	m_playerEntity = NULL;
	m_pathFinder = NULL;
	m_L = NULL;
	delete[] m_floodFillCounters;
	m_floodFillCounters = NULL;
	delete[] m_coords;
	m_coords = NULL;
	delete[] m_floodedBlocks;
	m_floodedBlocks = NULL;
}

void World::load() {
		if(m_d == NULL) {
			DbgPrintF("ERROR [RuntimeException]: %s\n", "The level is corrupt!");
		} else {
			m_worldAccesses = /* default vector */;
			m_heightMap = new int[width * m_length];
			Arrays::fill(m_heightMap, height);
			m_L->updateSkylight(0, 0, width, m_length);
			m_random = new Random();
			m_randId = m_random->nextInt();
			m_tickList = /* default vector */;
			if(m_entityMap == NULL) {
				m_entityMap = new EntityMap(width, height, m_length);
			}

		}
}

void World::generate(int var1, int var2, int var3, unsigned char* var4) {
		width = var1;
		m_length = var3;
		height = var2;
		m_d = var4;

		for(var2 = 0; var2 < width; ++var2) {
			for(int var5 = 0; var5 < m_length; ++var5) {
				for(int var6 = 0; var6 < height; ++var6) {
					int var7 = 0;
					if(var6 <= 1 && var6 < m_groundLevel - 1 && var4[((var6 + 1) * m_length + var5) * width + var2] == 0) {
						var7 = Block::s_lavaStill->blockID;
					} else if(var6 < m_groundLevel - 1) {
						var7 = Block::s_bedrock->blockID;
					} else if(var6 < m_groundLevel) {
						if(m_groundLevel > m_waterLevel && m_defaultFluid == Block::s_waterMoving->blockID) {
							var7 = Block::s_grass->blockID;
						} else {
							var7 = Block::s_dirt->blockID;
						}
					} else if(var6 < m_waterLevel) {
						var7 = m_defaultFluid;
					}

					var4[(var6 * m_length + var5) * width + var2] = (unsigned char)var7;
					if(var6 == 1 && var2 != 0 && var5 != 0 && var2 != width - 1 && var5 != m_length - 1) {
						var6 = height - 2;
					}
				}
			}
		}

		m_e = new unsigned char[var4 /* PORT: array m_length - use tracked size */];
		m_heightMap = new int[var1 * var3];
		Arrays::fill(m_heightMap, height);
		m_L = new Light(this);
		l();

		for(var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
			m_worldAccesses[var2]->loadRenderers();
		}

  memset(m_tickList, 0, sizeof(m_tickList));
		findSpawn();
		load();
		System.gc();
}

void World::a(int var1, int var2, int var3, unsigned char* var4, unsigned char* var5) {
		width = var1;
		m_length = var3;
		height = var2;
		m_d = var4;

		for(var2 = 0; var2 < width; ++var2) {
			for(int var6 = 0; var6 < m_length; ++var6) {
				for(int var7 = 0; var7 < height; ++var7) {
					int var8 = 0;
					if(var7 <= 1 && var7 < m_groundLevel - 1 && var4[((var7 + 1) * m_length + var6) * width + var2] == 0) {
						var8 = Block::s_lavaStill->blockID;
					} else if(var7 < m_groundLevel - 1) {
						var8 = Block::s_bedrock->blockID;
					} else if(var7 < m_groundLevel) {
						if(m_groundLevel > m_waterLevel && m_defaultFluid == Block::s_waterMoving->blockID) {
							var8 = Block::s_grass->blockID;
						} else {
							var8 = Block::s_dirt->blockID;
						}
					} else if(var7 < m_waterLevel) {
						var8 = m_defaultFluid;
					}

					var4[(var7 * m_length + var6) * width + var2] = (unsigned char)var8;
					if(var7 == 1 && var2 != 0 && var6 != 0 && var2 != width - 1 && var6 != m_length - 1) {
						var7 = height - 2;
					}
				}
			}
		}

		m_e = var5;
		m_heightMap = new int[var1 * var3];
		Arrays::fill(m_heightMap, height);
		m_L = new Light(this);
		l();

		for(var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
			m_worldAccesses[var2]->loadRenderers();
		}

  memset(m_tickList, 0, sizeof(m_tickList));
		findSpawn();
		load();
		System.gc();
}

void World::findSpawn() {
		Random* var1 = new Random();
		int var2 = 0;

		while(true) {
			label58:
			while(true) {
				int var3;
				size_t var4;
				size_t var5;
				do {
					do {
						++var2;
						var3 = var1->nextInt(width / 2) + width / 4;
						var4 = var1->nextInt(m_length / 2) + m_length / 4;
						var5 = getFirstUncoveredBlock(var3, var4) + 1;
						if(var2 == 1000000) {
							m_xSpawn = var3;
							m_ySpawn = height + 100;
							m_zSpawn = var4;
							return;
						}
					} while(var5 < 4);
				} while(var5 <= m_waterLevel);

				size_t var6;
				size_t var7;
				size_t var8;
				for(var6 = var3 - 3; var6 <= var3 + 3; ++var6) {
					for(var7 = var5 - 1; var7 <= var5 + 2; ++var7) {
						for(var8 = var4 - 3 - 2; var8 <= var4 + 3; ++var8) {
							if(getBlockMaterial(var6, var7, var8).isSolid()) {
								continue label58;
							}
						}
					}
				}

				var6 = var5 - 2;

				for(var7 = var3 - 3; var7 <= var3 + 3; ++var7) {
					for(var8 = var4 - 3 - 2; var8 <= var4 + 3; ++var8) {
						if(!Block::s_opaqueCubeLookup[getBlockId(var7, var6, var8)]) {
							continue label58;
						}
					}
				}

				m_xSpawn = var3;
				m_ySpawn = var5;
				m_zSpawn = var4;
				return;
			}
		}
}

void World::l() {
		int var1 = m_skylightSubtracted;

		for(int var2 = 0; var2 < width; ++var2) {
			for(int var3 = 0; var3 < m_length; ++var3) {
				size_t var4;
				for(var4 = height - 1; var4 > 0 && Block::s_lightOpacity[getBlockId(var2, var4, var3)] == 0; --var4) {
				}

				m_heightMap[var2 + var3 * width] = var4 + 1;

				for(var4 = 0; var4 < height; ++var4) {
					int var5 = (var4 * m_length + var3) * width + var2;
					int var6 = m_heightMap[var2 + var3 * width];
					var6 = var4 >= var6 ? var1 : 0;
					unsigned char var7 = m_d[var5];
					if(var6 < Block::s_lightValue[var7]) {
						var6 = Block::s_lightValue[var7];
					}

					m_e[var5] = (unsigned char)((m_e[var5] & 240) + var6);
				}
			}
		}

		m_L->updateBlockLight(0, 0, 0, width, height, m_length);
}

void World::addWorldAccess(IWorldAccess* var1) {
		for(int var2 = 0; var2 < m_entityMap->m_entities.size(); ++var2) {
			var1->obtainEntitySkin((Entity)m_entityMap->m_entities[var2]);
		}

		m_worldAccesses.push_back(var1);
}

void World::finalize() {
	// TODO: implement
}

void World::removeWorldAccess(IWorldAccess* var1) {
  std::vector<void*>::iterator _removeIt = std::find(m_worldAccesses.begin(), m_worldAccesses.end(), var1);
  if (_removeIt != m_worldAccesses.end()) m_worldAccesses.erase(_removeIt);
}

std::vector<void*> /* PORT: determine element type */ World::getCollidingBoundingBoxes(AxisAlignedBB var1) {
		std::vector<void*> var2 /* PORT: determine element type */;
		int var3 = (int)var1->minX;
		int var4 = (int)var1->maxX + 1;
		int var5 = (int)var1->minY;
		int var6 = (int)var1->maxY + 1;
		int var7 = (int)var1->minZ;
		int var8 = (int)var1->maxZ + 1;
		if(var1->minX < 0.0F) {
			--var3;
		}

		if(var1->minY < 0.0F) {
			--var5;
		}

		if(var1->minZ < 0.0F) {
			--var7;
		}

		for(var3 = var3; var3 < var4; ++var3) {
			for(int var9 = var5; var9 < var6; ++var9) {
				for(int var10 = var7; var10 < var8; ++var10) {
					Block var11 = Block::blocksList[getBlockId(var3, var9, var10)];
					AxisAlignedBB var12;
					if(var11 != NULL) {
						var12 = var11.getCollisionBoundingBoxFromPool(var3, var9, var10);
						if(var12 != NULL && var1->intersectsWith(var12)) {
							var2.push_back(var12);
						}
					} else if(m_groundLevel < 0 && (var9 < m_groundLevel || var9 < m_waterLevel)) {
						var12 = Block::s_bedrock->getCollisionBoundingBoxFromPool(var3, var9, var10);
						if(var12 != NULL && var1->intersectsWith(var12)) {
							var2.push_back(var12);
						}
					}
				}
			}
		}

		return var2;
}

void World::swap(int var1, int var2, int var3, int var4, int var5, int var6) {
		int var7 = getBlockId(var1, var2, var3);
		int var8 = getBlockId(var4, var5, var6);
		setBlock(var1, var2, var3, var8);
		setBlock(var4, var5, var6, var7);
		notifyBlocksOfNeighborChange(var1, var2, var3, var8);
		notifyBlocksOfNeighborChange(var4, var5, var6, var7);
}

bool World::setBlock(int var1, int var2, int var3, int var4) {
		if(var1 > 0 && var2 > 0 && var3 > 0 && var1 < width - 1 && var2 < height - 1 && var3 < m_length - 1) {
			if(var4 == m_d[(var2 * m_length + var3) * width + var1]) {
				return false;
			} else {
				if(var4 == 0 && (var1 == 0 || var3 == 0 || var1 == width - 1 || var3 == m_length - 1) && var2 >= m_groundLevel && var2 < m_waterLevel) {
					var4 = Block::s_waterMoving->blockID;
				}

				unsigned char var5 = m_d[(var2 * m_length + var3) * width + var1];
				m_d[(var2 * m_length + var3) * width + var1] = (unsigned char)var4;
				setBlockMetadata(var1, var2, var3, 0);
				if(var5 != 0) {
					Block::blocksList[var5].onBlockRemoval(*this, var1, var2, var3);
				}

				if(var4 != 0) {
					Block::blocksList[var4].onBlockAdded(*this, var1, var2, var3);
				}

				if(Block::s_lightOpacity[var5] != Block::s_lightOpacity[var4] || Block::s_lightValue[var5] != 0 || Block::s_lightValue[var4] != 0) {
					m_L->updateSkylight(var1, var3, 1, 1);
					m_L->updateBlockLight(var1, var2, var3, var1 + 1, var2 + 1, var3 + 1);
				}

				for(var4 = 0; var4 < m_worldAccesses.size(); ++var4) {
					m_worldAccesses[var4]->markBlockAndNeighborsNeedsUpdate(var1, var2, var3);
				}

				return true;
			}
		} else {
			return false;
		}
}

bool World::setBlockWithNotify(int var1, int var2, int var3, int var4) {
		if(setBlock(var1, var2, var3, var4)) {
			notifyBlocksOfNeighborChange(var1, var2, var3, var4);
			return true;
		} else {
			return false;
		}
}

void World::notifyBlocksOfNeighborChange(int var1, int var2, int var3, int var4) {
		notifyBlockOfNeighborChange(var1 - 1, var2, var3, var4);
		notifyBlockOfNeighborChange(var1 + 1, var2, var3, var4);
		notifyBlockOfNeighborChange(var1, var2 - 1, var3, var4);
		notifyBlockOfNeighborChange(var1, var2 + 1, var3, var4);
		notifyBlockOfNeighborChange(var1, var2, var3 - 1, var4);
		notifyBlockOfNeighborChange(var1, var2, var3 + 1, var4);
}

bool World::setTileNoUpdate(int var1, int var2, int var3, int var4) {
		if(var1 >= 0 && var2 >= 0 && var3 >= 0 && var1 < width && var2 < height && var3 < m_length) {
			if(var4 == m_d[(var2 * m_length + var3) * width + var1]) {
				return false;
			} else {
				m_d[(var2 * m_length + var3) * width + var1] = (unsigned char)var4;
				m_L->updateBlockLight(var1, var2, var3, var1 + 1, var2 + 1, var3 + 1);
				return true;
			}
		} else {
			return false;
		}
}

void World::notifyBlockOfNeighborChange(int var1, int var2, int var3, int var4) {
		if(var1 >= 0 && var2 >= 0 && var3 >= 0 && var1 < width && var2 < height && var3 < m_length) {
			Block var5 = Block::blocksList[m_d[(var2 * m_length + var3) * width + var1]];
			if(var5 != NULL) {
				var5.onNeighborBlockChange(*this, var1, var2, var3, var4);
			}

		}
}

int World::getBlockId(int var1, int var2, int var3) {
		if(var1 < 0) {
			var1 = 0;
		} else if(var1 >= width) {
			var1 = width - 1;
		}

		if(var2 < 0) {
			var2 = 0;
		} else if(var2 >= height) {
			var2 = height - 1;
		}

		if(var3 < 0) {
			var3 = 0;
		} else if(var3 >= m_length) {
			var3 = m_length - 1;
		}

		return m_d[(var2 * m_length + var3) * width + var1] & 255;
}

bool World::isBlockNormalCube(int var1, int var2, int var3) {
		Block var4 = Block::blocksList[getBlockId(var1, var2, var3)];
		return var4 == NULL ? false : var4.isOpaqueCube();
}

void World::updateEntities() {
		m_entityMap->updateEntities();
}

void World::updateLighting() {
		m_L->updateLight();
}

float World::getStarBrightness(float var1) {
		var1 = getCelestialAngle(var1);
		var1 = 1.0F - (cosf(var1 * 3.14159265358979323846f * 2.0F) * 2.0F + 12.0F / 16.0F);
		if(var1 < 0.0F) {
			var1 = 0.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		return var1 * var1 * 0.5F;
}

Vec3D* World::getSkyColor(float var1) {
		var1 = getCelestialAngle(var1);
		var1 = cosf(var1 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F;
		if(var1 < 0.0F) {
			var1 = 0.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		float var2 = (float)(m_skyColor >> 16 & 255) / 255.0F;
		float var3 = (float)(m_skyColor >> 8 & 255) / 255.0F;
		float var4 = (float)(m_skyColor & 255) / 255.0F;
		var2 *= var1;
		var3 *= var1;
		var4 *= var1;
		return new Vec3::0f(var2, var3, var4);
}

float World::getCelestialAngle(float var1) {
		var1 = ((float)m_worldTime + var1) / 24000.0F - 0.15F;
		return var1;
}

Vec3D* World::getFogColor(float var1) {
		var1 = getCelestialAngle(var1);
		var1 = cosf(var1 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F;
		if(var1 < 0.0F) {
			var1 = 0.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		var1 *= (float)(m_skyBrightness / 15);
		float var2 = (float)(m_fogColor >> 16 & 255) / 255.0F;
		float var3 = (float)(m_fogColor >> 8 & 255) / 255.0F;
		float var4 = (float)(m_fogColor & 255) / 255.0F;
		var2 *= var1 * 0.94F + 0.06F;
		var3 *= var1 * 0.94F + 0.06F;
		var4 *= var1 * 0.91F + 0.09F;
		return new Vec3::0f(var2, var3, var4);
}

Vec3D* World::getCloudColor(float var1) {
		var1 = getCelestialAngle(var1);
		var1 = cosf(var1 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F;
		if(var1 < 0.0F) {
			var1 = 0.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		var1 *= (float)(m_skyBrightness / 15);
		float var2 = (float)(m_cloudColor >> 16 & 255) / 255.0F;
		float var3 = (float)(m_cloudColor >> 8 & 255) / 255.0F;
		float var4 = (float)(m_cloudColor & 255) / 255.0F;
		var2 *= var1 * 0.9F + 0.1F;
		var3 *= var1 * 0.9F + 0.1F;
		var4 *= var1 * 0.85F + 0.15F;
		return new Vec3::0f(var2, var3, var4);
}

int World::getSkyBrightness() {
		float var1 = getCelestialAngle(1.0F);
		var1 = cosf(var1 * 3.14159265358979323846f * 2.0F) * 1.5F + 0.5F;
		if(var1 < 0.0F) {
			var1 = 0.0F;
		}

		if(var1 > 1.0F) {
			var1 = 1.0F;
		}

		int var2 = (int)(var1 * ((float)(15 * m_skyBrightness) / 15.0F - 4.0F) + 4.0F);
		if(var2 > 15) {
			var2 = 15;
		}

		if(var2 < 4) {
			var2 = 4;
		}

		return var2;
}

void World::tick() {
		++m_worldTime;
		if(m_worldTime == 24000) {
			m_worldTime = 0;
		}

		int var1 = getSkyBrightness();
		if(m_skylightSubtracted > var1) {
			updateChunkLight(m_skylightSubtracted - 1);
		}

		if(m_skylightSubtracted < var1) {
			updateChunkLight(m_skylightSubtracted + 1);
		}

		++m_playTime;
		var1 = 1;

		size_t var2;
		for(var2 = 1; 1 << var1 < width; ++var1) {
		}

		while(1 << var2 < m_length) {
			++var2;
		}

		int var3 = m_length - 1;
		int var4 = width - 1;
		int var5 = height - 1;
		int var6 = m_tickList.size();
		if(var6 > 200) {
			var6 = 200;
		}

		size_t var7;
		int var10;
		for(var7 = 0; var7 < var6; ++var7) {
   NextTickListEntry var8 = m_tickList[0];
   m_tickList.erase(m_tickList.begin() + 0);
			if(var8.m_scheduledTime > 0) {
				--var8.m_scheduledTime;
				m_tickList.push_back(var8);
			} else {
				int var12 = var8.m_zCoord;
				int var11 = var8.m_yCoord;
				var10 = var8.m_xCoord;
				if(var10 >= 0 && var11 >= 0 && var12 >= 0 && var10 < width && var11 < height && var12 < m_length) {
					unsigned char var9 = m_d[(var8.m_yCoord * m_length + var8.m_zCoord) * width + var8.m_xCoord];
					if(var9 == var8.blockID && var9 > 0) {
						Block::blocksList[var9].updateTick(*this, var8.m_xCoord, var8.m_yCoord, var8.m_zCoord, m_random);
					}
				}
			}
		}

		m_updateLCG += width * m_length * height;
		var6 = m_updateLCG / 200;
		m_updateLCG -= var6 * 200;

		for(var7 = 0; var7 < var6; ++var7) {
			m_randId = m_randId * 3 + 1013904223;
			int var13 = m_randId >> 2;
			int var14 = var13 & var4;
			var10 = var13 >> var1 & var3;
			var13 = var13 >> var1 + var2 & var5;
			unsigned char var15 = m_d[(var13 * m_length + var10) * width + var14];
			if(Block::s_tickOnLoad[var15]) {
				Block::blocksList[var15].updateTick(*this, var14, var13, var10, m_random);
			}
		}

}

int World::entitiesInLevelList(Class* var1) {
		int var2 = 0;

		for(int var3 = 0; var3 < m_entityMap->m_entities.size(); ++var3) {
			Entity var4 = (Entity)m_entityMap->m_entities[var3];
			if(var1->isAssignableFrom(var4.getClass())) {
				++var2;
			}
		}

		return var2;
}

int World::getGroundLevel() {
		return m_groundLevel;
}

int World::getWaterLevel() {
		return m_waterLevel;
}

bool World::getIsAnyLiquid(AxisAlignedBB var1) {
		int var2 = (int)var1.minX;
		int var3 = (int)var1.maxX + 1;
		int var4 = (int)var1.minY;
		int var5 = (int)var1.maxY + 1;
		int var6 = (int)var1.minZ;
		int var7 = (int)var1.maxZ + 1;
		if(var1.minX < 0.0F) {
			--var2;
		}

		if(var1.minY < 0.0F) {
			--var4;
		}

		if(var1.minZ < 0.0F) {
			--var6;
		}

		if(var2 < 0) {
			var2 = 0;
		}

		if(var4 < 0) {
			var4 = 0;
		}

		if(var6 < 0) {
			var6 = 0;
		}

		if(var3 > width) {
			var3 = width;
		}

		if(var5 > height) {
			var5 = height;
		}

		if(var7 > m_length) {
			var7 = m_length;
		}

		for(int var10 = var2; var10 < var3; ++var10) {
			for(var2 = var4; var2 < var5; ++var2) {
				for(int var8 = var6; var8 < var7; ++var8) {
					Block var9 = Block::blocksList[getBlockId(var10, var2, var8)];
					if(var9 != NULL && var9.material->getIsLiquid()) {
						return true;
					}
				}
			}
		}

		return false;
}

bool World::isBoundingBoxBurning(AxisAlignedBB var1) {
		int var2 = (int)var1.minX;
		int var3 = (int)var1.maxX + 1;
		int var4 = (int)var1.minY;
		int var5 = (int)var1.maxY + 1;
		int var6 = (int)var1.minZ;
		int var10 = (int)var1.maxZ + 1;

		for(var2 = var2; var2 < var3; ++var2) {
			for(int var7 = var4; var7 < var5; ++var7) {
				for(int var8 = var6; var8 < var10; ++var8) {
					int var9 = getBlockId(var2, var7, var8);
					if(var9 == Block::s_fire->blockID || var9 == Block::s_lavaMoving->blockID || var9 == Block::s_lavaStill->blockID) {
						return true;
					}
				}
			}
		}

		return false;
}

bool World::handleMaterialAcceleration(AxisAlignedBB var1, Material* var2) {
		int var3 = (int)var1.minX;
		int var4 = (int)var1.maxX + 1;
		int var5 = (int)var1.minY;
		int var6 = (int)var1.maxY + 1;
		int var7 = (int)var1.minZ;
		int var11 = (int)var1.maxZ + 1;

		for(var3 = var3; var3 < var4; ++var3) {
			for(int var8 = var5; var8 < var6; ++var8) {
				for(int var9 = var7; var9 < var11; ++var9) {
					Block var10 = Block::blocksList[getBlockId(var3, var8, var9)];
					if(var10 != NULL && var10.material == var2) {
						return true;
					}
				}
			}
		}

		return false;
}

void World::scheduleBlockUpdate(int var1, int var2, int var3, int var4) {
		NextTickListEntry var5 = NextTickListEntry(var1, var2, var3, var4);
		if(var4 > 0) {
			var3 = Block::blocksList[var4].tickRate();
			var5.m_scheduledTime = var3;
		}

		m_tickList.push_back(var5);
}

bool World::checkIfAABBIsClear1(AxisAlignedBB var1) {
		return m_entityMap->a((Entity)NULL, var1).size() == 0;
}

bool World::checkIfAABBIsClear(AxisAlignedBB var1) {
		std::vector<Entity> var4 = m_entityMap->a((Entity)NULL, var1);

		for(size_t var2 = 0; var2 < var4.size(); ++var2) {
			Entity var3 = (Entity)var4[var2];
			if(var3.preventEntitySpawning) {
				return false;
			}
		}

		return true;
}

std::vector<void*> /* PORT: determine element type */ World::a(Entity* var1, AxisAlignedBB var2) {
		return m_entityMap->a(var1, var2);
}

bool World::isSolid(float var1, float var2, float var3, float var4) {
		return isSolid(var1 - 0.1F, var2 - 0.1F, var3 - 0.1F) ? true : (isSolid(var1 - 0.1F, var2 - 0.1F, var3 + 0.1F) ? true : (isSolid(var1 - 0.1F, var2 + 0.1F, var3 - 0.1F) ? true : (isSolid(var1 - 0.1F, var2 + 0.1F, var3 + 0.1F) ? true : (isSolid(var1 + 0.1F, var2 - 0.1F, var3 - 0.1F) ? true : (isSolid(var1 + 0.1F, var2 - 0.1F, var3 + 0.1F) ? true : (isSolid(var1 + 0.1F, var2 + 0.1F, var3 - 0.1F) ? true : isSolid(var1 + 0.1F, var2 + 0.1F, var3 + 0.1F)))))));
}

bool World::isSolid(float var1, float var2, float var3) {
		int var4 = getBlockId((int)var1, (int)var2, (int)var3);
		return var4 > 0 && Block::blocksList[var4].isOpaqueCube();
}

int World::getFirstUncoveredBlock(int var1, int var2) {
		int var3;
		for(var3 = height; (getBlockId(var1, var3 - 1, var2) == 0 || Block::blocksList[getBlockId(var1, var3 - 1, var2)].material == Material::s_air) && var3 > 0; --var3) {
		}

		return var3;
}

void World::setSpawnLocation(int var1, int var2, int var3, float var4) {
		m_xSpawn = var1;
		m_ySpawn = var2;
		m_zSpawn = var3;
		m_rotSpawn = var4;
}

float World::getLightBrightness(int var1, int var2, int var3) {
		return s_lightBrightnessTable[getBlockLightValue(var1, var2, var3)];
}

unsigned char World::getBlockLightValue(int var1, int var2, int var3) {
		if(var1 < 0) {
			var1 = 0;
		} else if(var1 >= width) {
			var1 = width - 1;
		}

		if(var2 < 0) {
			var2 = 0;
		} else if(var2 >= height) {
			var2 = height - 1;
		}

		if(var3 < 0) {
			var3 = 0;
		} else if(var3 >= m_length) {
			var3 = m_length - 1;
		}

		return m_d[(var2 * m_length + var3) * width + var1] == Block::s_stairSingle->blockID ? (var2 < height - 1 ? (unsigned char)(m_e[((var2 + 1) * m_length + var3) * width + var1] & 15) : 15) : (unsigned char)(m_e[(var2 * m_length + var3) * width + var1] & 15);
}

unsigned char World::getBlockMetadata(int var1, int var2, int var3) {
		if(var1 < 0) {
			var1 = 0;
		} else if(var1 >= width) {
			var1 = width - 1;
		}

		if(var2 < 0) {
			var2 = 0;
		} else if(var2 >= height) {
			var2 = height - 1;
		}

		if(var3 < 0) {
			var3 = 0;
		} else if(var3 >= m_length) {
			var3 = m_length - 1;
		}

		return (unsigned char)(m_e[(var2 * m_length + var3) * width + var1] >>> 4 & 15);
}

void World::setBlockMetadata(int var1, int var2, int var3, int var4) {
		if(var1 < 0) {
			var1 = 0;
		} else if(var1 >= width) {
			var1 = width - 1;
		}

		if(var2 < 0) {
			var2 = 0;
		} else if(var2 >= height) {
			var2 = height - 1;
		}

		if(var3 < 0) {
			var3 = 0;
		} else if(var3 >= m_length) {
			var3 = m_length - 1;
		}

		m_e[(var2 * m_length + var3) * width + var1] = (unsigned char)((m_e[(var2 * m_length + var3) * width + var1] & 15) + (var4 << 4));

		for(var4 = 0; var4 < m_worldAccesses.size(); ++var4) {
			m_worldAccesses[var4]->markBlockAndNeighborsNeedsUpdate(var1, var2, var3);
		}

}

Material* World::getBlockMaterial(int var1, int var2, int var3) {
		var1 = getBlockId(var1, var2, var3);
		return var1 == 0 ? Material::s_air : Block::blocksList[var1].material;
}

bool World::isWater(int var1, int var2, int var3) {
		var1 = getBlockId(var1, var2, var3);
		return var1 > 0 && Block::blocksList[var1].material == Material::s_water;
}

MovingObjectPosition* World::rayTraceBlocks(Vec3D* var1, Vec3D* var2) {
		if(!Float.isNaN(var1->m_xCoord) && !Float.isNaN(var1->m_yCoord) && !Float.isNaN(var1->m_zCoord)) {
			if(!Float.isNaN(var2->m_xCoord) && !Float.isNaN(var2->m_yCoord) && !Float.isNaN(var2->m_zCoord)) {
				int var3 = (int)floorf(var2->m_xCoord);
				int var4 = (int)floorf(var2->m_yCoord);
				int var5 = (int)floorf(var2->m_zCoord);
				int var6 = (int)floorf(var1->m_xCoord);
				int var7 = (int)floorf(var1->m_yCoord);
				int var8 = (int)floorf(var1->m_zCoord);
				int var9 = 20;

				while(var9-- >= 0) {
					if(Float.isNaN(var1->m_xCoord) || Float.isNaN(var1->m_yCoord) || Float.isNaN(var1->m_zCoord)) {
						return NULL;
					}

					if(var6 == var3 && var7 == var4 && var8 == var5) {
						return NULL;
					}

					float var10 = 999.0F;
					float var11 = 999.0F;
					float var12 = 999.0F;
					if(var3 > var6) {
						var10 = (float)var6 + 1.0F;
					}

					if(var3 < var6) {
						var10 = (float)var6;
					}

					if(var4 > var7) {
						var11 = (float)var7 + 1.0F;
					}

					if(var4 < var7) {
						var11 = (float)var7;
					}

					if(var5 > var8) {
						var12 = (float)var8 + 1.0F;
					}

					if(var5 < var8) {
						var12 = (float)var8;
					}

					float var13 = 999.0F;
					float var14 = 999.0F;
					float var15 = 999.0F;
					float var16 = var2->m_xCoord - var1->m_xCoord;
					float var17 = var2->m_yCoord - var1->m_yCoord;
					float var18 = var2->m_zCoord - var1->m_zCoord;
					if(var10 != 999.0F) {
						var13 = (var10 - var1->m_xCoord) / var16;
					}

					if(var11 != 999.0F) {
						var14 = (var11 - var1->m_yCoord) / var17;
					}

					if(var12 != 999.0F) {
						var15 = (var12 - var1->m_zCoord) / var18;
					}

					unsigned char var19;
					if(var13 < var14 && var13 < var15) {
						if(var3 > var6) {
							var19 = 4;
						} else {
							var19 = 5;
						}

						var1->m_xCoord = var10;
						var1->m_yCoord += var17 * var13;
						var1->m_zCoord += var18 * var13;
					} else if(var14 < var15) {
						if(var4 > var7) {
							var19 = 0;
						} else {
							var19 = 1;
						}

						var1->m_xCoord += var16 * var14;
						var1->m_yCoord = var11;
						var1->m_zCoord += var18 * var14;
					} else {
						if(var5 > var8) {
							var19 = 2;
						} else {
							var19 = 3;
						}

						var1->m_xCoord += var16 * var15;
						var1->m_yCoord += var17 * var15;
						var1->m_zCoord = var12;
					}

					Vec3.0f var20 = new Vec3::0f(var1->m_xCoord, var1->m_yCoord, var1->m_zCoord);
					var6 = (int)(var20.m_xCoord = (float)(int)floorf(var1->m_xCoord));
					if(var19 == 5) {
						--var6;
						++var20.m_xCoord;
					}

					var7 = (int)(var20.m_yCoord = (float)(int)floorf(var1->m_yCoord));
					if(var19 == 1) {
						--var7;
						++var20.m_yCoord;
					}

					var8 = (int)(var20.m_zCoord = (float)(int)floorf(var1->m_zCoord));
					if(var19 == 3) {
						--var8;
						++var20.m_zCoord;
					}

					int var21 = getBlockId(var6, var7, var8);
					Block var23 = Block::blocksList[var21];
					if(var21 > 0 && var23.isCollidable()) {
						MovingObjectPosition var22 = var23.collisionRayTrace(*this, var6, var7, var8, var1, var2);
						if(var22 != NULL) {
							return var22;
						}
					}
				}

				return NULL;
			} else {
				return NULL;
			}
		} else {
			return NULL;
		}
}

bool World::growTrees(int var1, int var2, int var3) {
		int var4 = m_random->nextInt(3) + 4;
		bool var5 = true;
		if(var2 > 0 && var2 + var4 + 1 <= height) {
			size_t var6;
			size_t var8;
			int var9;
			int var10;
			for(var6 = var2; var6 <= var2 + 1 + var4; ++var6) {
				unsigned char var7 = 1;
				if(var6 == var2) {
					var7 = 0;
				}

				if(var6 >= var2 + 1 + var4 - 2) {
					var7 = 2;
				}

				for(var8 = var1 - var7; var8 <= var1 + var7 && var5; ++var8) {
					for(var9 = var3 - var7; var9 <= var3 + var7 && var5; ++var9) {
						if(var8 >= 0 && var6 >= 0 && var9 >= 0 && var8 < width && var6 < height && var9 < m_length) {
							var10 = m_d[(var6 * m_length + var9) * width + var8] & 255;
							if(var10 != 0) {
								var5 = false;
							}
						} else {
							var5 = false;
						}
					}
				}
			}

			if(!var5) {
				return false;
			} else {
				var6 = m_d[((var2 - 1) * m_length + var3) * width + var1] & 255;
				if((var6 == Block::s_grass->blockID || var6 == Block::s_dirt->blockID) && var2 < height - var4 - 1) {
					setBlockWithNotify(var1, var2 - 1, var3, Block::s_dirt->blockID);

					int var13;
					for(var13 = var2 - 3 + var4; var13 <= var2 + var4; ++var13) {
						var8 = var13 - (var2 + var4);
						var9 = 1 - var8 / 2;

						for(var10 = var1 - var9; var10 <= var1 + var9; ++var10) {
							int var12 = var10 - var1;

							for(var6 = var3 - var9; var6 <= var3 + var9; ++var6) {
								int var11 = var6 - var3;
								if((abs(var12) != var9 || abs(var11) != var9 || m_random->nextInt(2) != 0 && var8 != 0) && !Block::s_opaqueCubeLookup[getBlockId(var10, var13, var6)]) {
									setBlockWithNotify(var10, var13, var6, Block::s_leaves->blockID);
								}
							}
						}
					}

					for(var13 = 0; var13 < var4; ++var13) {
						if(!Block::s_opaqueCubeLookup[getBlockId(var1, var2 + var13, var3)]) {
							setBlockWithNotify(var1, var2 + var13, var3, Block::s_wood->blockID);
						}
					}

					return true;
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
}

Entity* World::getPlayerEntity() {
		return m_playerEntity;
}

void World::spawnEntityInWorld(Entity* var1) {
		m_entityMap->insert(var1);
		var1->setWorld(this);

		for(size_t var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
			m_worldAccesses[var2]->obtainEntitySkin(var1);
		}

}

void World::releaseEntitySkin(Entity* var1) {
		m_entityMap->erase(.begin() + var1) /* PORT: verify remove semantics */;

		for(size_t var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
			m_worldAccesses[var2]->releaseEntitySkin(var1);
		}

}

void World::createExplosion(Entity* var1, float var2, float var3, float var4, float var5) {
		playSoundAtPlayer(var2, var3, var4, "m_random->explode", 4.0F, (1.0F + (m_random->nextFloat() - m_random->nextFloat()) * 0.2F) * 0.7F);
		TreeSet* var6 = new TreeSet();
		float var7 = var5;

		size_t var8;
		int var9;
		int var10;
		float var11;
		float var18;
		int var19;
		int var20;
		int var21;
		int var22;
		for(var8 = 0; var8 < 16; ++var8) {
			for(var9 = 0; var9 < 16; ++var9) {
				for(var10 = 0; var10 < 16; ++var10) {
					if(var8 == 0 || var8 == 15 || var9 == 0 || var9 == 15 || var10 == 0 || var10 == 15) {
						var11 = (float)var8 / 15.0F * 2.0F - 1.0F;
						float var12 = (float)var9 / 15.0F * 2.0F - 1.0F;
						float var13 = (float)var10 / 15.0F * 2.0F - 1.0F;
						float var14 = (float)sqrtf((double)(var11 * var11 + var12 * var12 + var13 * var13));
						var11 /= var14;
						var12 /= var14;
						var13 /= var14;
						float var15 = var5 * (0.7F + m_random->nextFloat() * 0.6F);
						float var16 = var2;
						float var17 = var3;

						for(var18 = var4; var15 > 0.0F; var15 -= 0.22500001F) {
							var19 = (int)var16;
							var20 = (int)var17;
							var21 = (int)var18;
							var22 = getBlockId(var19, var20, var21);
							if(var22 > 0) {
								var15 -= (Block::blocksList[var22].getExplosionResistance() + 0.3F) * 0.3F;
							}

							if(var15 > 0.0F) {
								int var23 = var19 + (var20 << 10) + (var21 << 10 << 10);
								var6->push_back(var23);
							}

							var16 += var11 * 0.3F;
							var17 += var12 * 0.3F;
							var18 += var13 * 0.3F;
						}
					}
				}
			}
		}

		var5 *= 2.0F;
		var8 = (int)(var2 - var5 - 1.0F);
		var9 = (int)(var2 + var5 + 1.0F);
		var10 = (int)(var3 - var5 - 1.0F);
		int var29 = (int)(var3 + var5 + 1.0F);
		int var30 = (int)(var4 - var5 - 1.0F);
		int var31 = (int)(var4 + var5 + 1.0F);
		std::vector<Entity> var32 = m_entityMap->a(var1, (float)var8, (float)var10, (float)var30, (float)var9, (float)var29, (float)var31);
		Vec3.0f var33 = new Vec3::0f(var2, var3, var4);

		float var27;
		float var28;
		float var43;
		for(size_t var34 = 0; var34 < var32.size(); ++var34) {
			Entity var36 = (Entity)var32[var34];
			var27 = var36.posX - var2;
			var28 = var36.posY - var3;
			float var26 = var36.posZ - var4;
			var18 = sqrtf(var27 * var27 + var28 * var28 + var26 * var26) / var5;
			if(var18 <= 1.0F) {
				var26 = var36.posX - var2;
				float var39 = var36.posY - var3;
				float var40 = var36.posZ - var4;
				float var41 = sqrtf(var26 * var26 + var39 * var39 + var40 * var40);
				var26 /= var41;
				var39 /= var41;
				var40 /= var41;
				float var42 = getBlockDensity(var33, var36.boundingBox);
				var43 = (1.0F - var18) * var42;
				var36.attackEntityFrom(var1, (int)((var43 * var43 + var43) / 2.0F * 8.0F * var5 + 1.0F));
				var36.motionX += var26 * var43;
				var36.motionY += var39 * var43;
				var36.motionZ += var40 * var43;
			}
		}

		var5 = var7;
		std::vector<void*> var35 /* PORT: determine element type */;
		var35.addAll(var6);

		for(int var37 = var35.size() - 1; var37 >= 0; --var37) {
			int var38 = var35[var37]->intValue();
			var8 = var38 & 1023;
			var19 = var38 >> 10 & 1023;
			var20 = var38 >> 20 & 1023;
			if(var8 >= 0 && var19 >= 0 && var20 >= 0 && var8 < width && var19 < height && var20 < m_length) {
				var21 = getBlockId(var8, var19, var20);

				for(var22 = 0; var22 <= 0; ++var22) {
					var43 = (float)var8 + m_random->nextFloat();
					var27 = (float)var19 + m_random->nextFloat();
					float var24 = (float)var20 + m_random->nextFloat();
					float var25 = var43 - var2;
					var7 = var27 - var3;
					var28 = var24 - var4;
					var11 = sqrtf(var25 * var25 + var7 * var7 + var28 * var28);
					var25 /= var11;
					var7 /= var11;
					var28 /= var11;
					var11 = 0.5F / (var11 / var5 + 0.1F);
					var11 *= m_random->nextFloat() * m_random->nextFloat() + 0.3F;
					var25 *= var11;
					var7 *= var11;
					var28 *= var11;
					spawnParticle("explode", (var43 + var2) / 2.0F, (var27 + var3) / 2.0F, (var24 + var4) / 2.0F, var25, var7, var28);
					spawnParticle("smoke", var43, var27, var24, var25, var7, var28);
				}

				if(var21 > 0) {
					Block::blocksList[var21].dropBlockAsItemWithChance(*this, var8, var19, var20, getBlockMetadata(var8, var19, var20), 0.3F);
					setBlockWithNotify(var8, var19, var20, 0);
					Block::blocksList[var21].onBlockDestroyedByExplosion(*this, var8, var19, var20);
				}
			}
		}

}

float World::getBlockDensity(Vec3D* var1, AxisAlignedBB var2) {
		float var3 = 1.0F / ((var2.maxX - var2.minX) * 2.0F + 1.0F);
		float var4 = 1.0F / ((var2.maxY - var2.minY) * 2.0F + 1.0F);
		float var5 = 1.0F / ((var2.maxZ - var2.minZ) * 2.0F + 1.0F);
		int var6 = 0;
		int var7 = 0;

		for(float var8 = 0.0F; var8 <= 1.0F; var8 += var3) {
			for(float var9 = 0.0F; var9 <= 1.0F; var9 += var4) {
				for(float var10 = 0.0F; var10 <= 1.0F; var10 += var5) {
					float var11 = var2.minX + (var2.maxX - var2.minX) * var8;
					float var12 = var2.minY + (var2.maxY - var2.minY) * var9;
					float var13 = var2.minZ + (var2.maxZ - var2.minZ) * var10;
					if(rayTraceBlocks(new Vec3::0f(var11, var12, var13), var1) == NULL) {
						++var6;
					}

					++var7;
				}
			}
		}

		return (float)var6 / (float)var7;
}

Entity* World::findSubclassOf(Class* var1) {
		for(int var2 = 0; var2 < m_entityMap->m_entities.size(); ++var2) {
			Entity var3 = (Entity)m_entityMap->m_entities[var2];
			if(var1->isAssignableFrom(var3.getClass())) {
				return var3;
			}
		}

		return NULL;
}

int World::a(int var1, int var2) {
		return m_heightMap[var1 + var2 * width];
}

int World::fluidFlowCheck(int var1, int var2, int var3, int var4, int var5) {
		if(var1 >= 0 && var2 >= 0 && var3 >= 0 && var1 < width && var2 < height && var3 < m_length) {
			int var6 = var1;
			int var7 = var3;
			int var8 = ((var2 << 10) + var3 << 10) + var1;
			unsigned char var9 = 0;
			int var20 = var9 + 1;
			m_coords[0] = var1 + (var3 << 10);
			int var11 = -9999;
			if(var4 == Block::s_waterStill->blockID || var4 == Block::s_waterMoving->blockID) {
				var11 = Block.waterSource.blockID;
			}

			if(var4 == Block::s_lavaStill->blockID || var4 == Block::s_lavaMoving->blockID) {
				var11 = Block.lavaSource.blockID;
			}

			int var10;
			bool var12;
			label170:
			do {
				var12 = false;
				int var13 = -1;
				var10 = 0;
				if(++s_floodFillCounter == 30000) {
					Arrays::fill(m_floodFillCounters, (short)0);
					s_floodFillCounter = 1;
				}

				while(true) {
					int var14;
					do {
						if(var20 <= 0) {
							++var2;
							int[] var21 = m_floodedBlocks;
							m_floodedBlocks = m_coords;
							m_coords = var21;
							var20 = var10;
							continue label170;
						}

						--var20;
						var14 = m_coords[var20];
					} while(m_floodFillCounters[var14] == s_floodFillCounter);

					var1 = var14 % 1024;
					var3 = var14 / 1024;
					int var15 = var3 - var7;

					for(var15 *= var15; var1 > 0 && m_floodFillCounters[var14 - 1] != s_floodFillCounter && (m_d[(var2 * m_length + var3) * width + var1 - 1] == var4 || m_d[(var2 * m_length + var3) * width + var1 - 1] == var5); --var14) {
						--var1;
					}

					if(var1 > 0 && m_d[(var2 * m_length + var3) * width + var1 - 1] == var11) {
						var12 = true;
					}

					bool var16 = false;
					bool var17 = false;

					for(bool var18 = false; var1 < width && m_floodFillCounters[var14] != s_floodFillCounter && (m_d[(var2 * m_length + var3) * width + var1] == var4 || m_d[(var2 * m_length + var3) * width + var1] == var5); ++var1) {
						unsigned char var19;
						bool var22;
						if(var3 > 0) {
							var19 = m_d[(var2 * m_length + var3 - 1) * width + var1];
							if(var19 == var11) {
								var12 = true;
							}

							var22 = m_floodFillCounters[var14 - 1024] != s_floodFillCounter && (var19 == var4 || var19 == var5);
							if(var22 && !var16) {
								m_coords[var20++] = var14 - 1024;
							}

							var16 = var22;
						}

						if(var3 < m_length - 1) {
							var19 = m_d[(var2 * m_length + var3 + 1) * width + var1];
							if(var19 == var11) {
								var12 = true;
							}

							var22 = m_floodFillCounters[var14 + 1024] != s_floodFillCounter && (var19 == var4 || var19 == var5);
							if(var22 && !var17) {
								m_coords[var20++] = var14 + 1024;
							}

							var17 = var22;
						}

						if(var2 < height - 1) {
							var19 = m_d[((var2 + 1) * m_length + var3) * width + var1];
							var22 = var19 == var4 || var19 == var5;
							if(var22 && !var18) {
								m_floodedBlocks[var10++] = var14;
							}

							var18 = var22;
						}

						int var23 = var1 - var6;
						var23 *= var23;
						var23 += var15;
						if(var23 > var13) {
							var13 = var23;
							var8 = ((var2 << 10) + var3 << 10) + var1;
						}

						m_floodFillCounters[var14++] = s_floodFillCounter;
					}

					if(var1 < width && m_d[(var2 * m_length + var3) * width + var1] == var11) {
						var12 = true;
					}
				}
			} while(var10 > 0);

			if(var12) {
				return -9999;
			} else {
				return var8;
			}
		} else {
			return -1;
		}
}

int World::floodFill(int var1, int var2, int var3, int var4, int var5) {
		if(var1 >= 0 && var2 >= 0 && var3 >= 0 && var1 < width && var2 < height && var3 < m_length) {
			if(++s_floodFillCounter == 30000) {
				Arrays::fill(m_floodFillCounters, (short)0);
				s_floodFillCounter = 1;
			}

			unsigned char var6 = 0;
			int var11 = var6 + 1;
			m_coords[0] = var1 + (var3 << 10);

			do {
				size_t var7;
				do {
					if(var11 <= 0) {
						return 1;
					}

					--var11;
					var7 = m_coords[var11];
				} while(m_floodFillCounters[var7] == s_floodFillCounter);

				var1 = var7 % 1024;
				var3 = var7 / 1024;
				if(var1 == 0 || var1 == width - 1 || var2 == 0 || var2 == height - 1 || var3 == 0 || var3 == m_length - 1) {
					return 2;
				}

				while(var1 > 0 && m_floodFillCounters[var7 - 1] != s_floodFillCounter && (m_d[(var2 * m_length + var3) * width + var1 - 1] == var4 || m_d[(var2 * m_length + var3) * width + var1 - 1] == var5)) {
					--var1;
					--var7;
				}

				if(var1 > 0 && m_d[(var2 * m_length + var3) * width + var1 - 1] == 0) {
					return 0;
				}

				bool var8 = false;

				for(bool var9 = false; var1 < width && m_floodFillCounters[var7] != s_floodFillCounter && (m_d[(var2 * m_length + var3) * width + var1] == var4 || m_d[(var2 * m_length + var3) * width + var1] == var5); ++var1) {
					if(var1 == 0 || var1 == width - 1) {
						return 2;
					}

					unsigned char var10;
					bool var12;
					if(var3 > 0) {
						var10 = m_d[(var2 * m_length + var3 - 1) * width + var1];
						if(var10 == 0) {
							return 0;
						}

						var12 = m_floodFillCounters[var7 - 1024] != s_floodFillCounter && (var10 == var4 || var10 == var5);
						if(var12 && !var8) {
							m_coords[var11++] = var7 - 1024;
						}

						var8 = var12;
					}

					if(var3 < m_length - 1) {
						var10 = m_d[(var2 * m_length + var3 + 1) * width + var1];
						if(var10 == 0) {
							return 0;
						}

						var12 = m_floodFillCounters[var7 + 1024] != s_floodFillCounter && (var10 == var4 || var10 == var5);
						if(var12 && !var9) {
							m_coords[var11++] = var7 + 1024;
						}

						var9 = var12;
					}

					m_floodFillCounters[var7] = s_floodFillCounter;
					++var7;
				}
			} while(var1 >= width || m_d[(var2 * m_length + var3) * width + var1] != 0);

			return 0;
		} else {
			return 0;
		}
}

void World::a(Entity* var1, const std::string& var2, float var3, float var4) {
		for(size_t var5 = 0; var5 < m_worldAccesses.size(); ++var5) {
			float var6 = 16.0F;
			if(var3 > 1.0F) {
				var6 = 16.0F * var3;
			}

			if(m_playerEntity->getDistanceSqToEntity(var1) < var6 * var6) {
				m_worldAccesses[var5]->playSound(var2, var1->posX, var1->posY - var1->yOffset, var1->posZ, var3, var4);
			}
		}

}

void World::playSoundEffect(float var1, float var2, float var3, const std::string& var4, float var5) {
  { // was try
			for(size_t var7 = 0; var7 < m_worldAccesses.size(); ++var7) {
				m_worldAccesses[var7]->playMusic(var4, var1, var2, var3, 0.0F);
			}

		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var6)
   // Removed Java stream/exception call: var6.printStackTrace();
		}
}

void World::playSoundAtPlayer(float var1, float var2, float var3, const std::string& var4, float var5, float var6) {
  { // was try
			for(size_t var7 = 0; var7 < m_worldAccesses.size(); ++var7) {
				float var8 = 16.0F;
				if(var5 > 1.0F) {
					var8 = 16.0F * var5;
				}

				float var9 = var1 - m_playerEntity->posX;
				float var10 = var2 - m_playerEntity->posY;
				float var11 = var3 - m_playerEntity->posZ;
				if(var9 * var9 + var10 * var10 + var11 * var11 < var8 * var8) {
					m_worldAccesses[var7]->playSound(var4, var1, var2, var3, var5, var6);
				}
			}

		} { DbgPrintF("CATCH Exception\n"); // was catch(Exception var12)
   // Removed Java stream/exception call: var12.printStackTrace();
		}
}

void World::extinguishFire(int var1, int var2, int var3, int var4) {
		if(var4 == 0) {
			--var2;
		}

		if(var4 == 1) {
			++var2;
		}

		if(var4 == 2) {
			--var3;
		}

		if(var4 == 3) {
			++var3;
		}

		if(var4 == 4) {
			--var1;
		}

		if(var4 == 5) {
			++var1;
		}

		if(getBlockId(var1, var2, var3) == Block::s_fire->blockID) {
			playSoundAtPlayer((float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, "m_random->fizz", 0.5F, 2.6F + (m_random->nextFloat() - m_random->nextFloat()) * 0.8F);
			setBlockWithNotify(var1, var2, var3, 0);
		}

}

void World::setBlockTileEntity(int var1, int var2, int var3, TileEntity* var4) {
		m_map->put(var1 + (var2 << 10 + (var3 << 10 << 10)), var4);
}

void World::removeBlockTileEntity(int var1, int var2, int var3) {
		m_map->remove(var1 + (var2 << 10 + (var3 << 10 << 10)));
}

TileEntity* World::getBlockTileEntity(int var1, int var2, int var3) {
		return (TileEntity)m_map->get(var1 + (var2 << 10 + (var3 << 10 << 10)));
}

void World::spawnParticle(const std::string& var1, float var2, float var3, float var4, float var5, float var6, float var7) {
		for(size_t var8 = 0; var8 < m_worldAccesses.size(); ++var8) {
			m_worldAccesses[var8]->spawnParticle(var1, var2, var3, var4, var5, var6, var7);
		}

}

void World::randomDisplayUpdates(int var1, int var2, int var3) {
		for(int var4 = 0; var4 < 1000; ++var4) {
			int var5 = var1 + m_random->nextInt(16) - m_random->nextInt(16);
			int var6 = var2 + m_random->nextInt(16) - m_random->nextInt(16);
			int var7 = var3 + m_random->nextInt(16) - m_random->nextInt(16);
			int var8 = getBlockId(var5, var6, var7);
			if(var8 > 0) {
				Block::blocksList[var8].randomDisplayTick(*this, var5, var6, var7, rand);
			}
		}

}

const std::string& World::debugSkylightUpdates() {
		return "" + m_tickList.size() + ". m_L: " + m_L->debugSkylightUpdates();
}

void World::setLevel() {
		for(size_t var1 = 0; var1 < m_worldAccesses.size(); ++var1) {
			IWorldAccess var2 = (IWorldAccess)m_worldAccesses[var1];

			for(int var3 = 0; var3 < m_entityMap->m_entities.size(); ++var3) {
				var2.releaseEntitySkin((Entity)m_entityMap->m_entities[var1]);
			}
		}

}

void World::updateChunkLight(int var1) {
		m_L->updateDaylightCycle(var1);
}

bool World::canBlockSeeTheSky(int var1, int var2, int var3) {
		if(m_heightMap[var1 + var3 * width] <= var2) {
			return true;
		} else {
			while(var2 < height) {
				if(Block::s_opaqueCubeLookup[getBlockId(var1, var2, var3)]) {
					return false;
				}

				++var2;
			}

			return true;
		}
}

void World::initStatics() {
		for(int var0 = 0; var0 <= 15; ++var0) {
			float var1 = 1.0F - (float)var0 / 15.0F;
			s_lightBrightnessTable[var0] = (1.0F - var1) / (var1 * 3.0F + 1.0F) * 0.95F + 0.05F;
		}

		s_floodFillCounter = 0;
}
