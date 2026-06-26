#include "stdafx.h"
// Auto-generated from World.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "World.h"
#include "Common.h"
#include "BlockFire.h"
#include "AxisAlignedBB.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "EnumSkyBlock.h"
#include "Explosion.h"
#include "IChunkProvider.h"
#include "IProgressUpdate.h"
#include "IWorldAccess.h"
#include "Material.h"
#include "MovingObjectPosition.h"
#include "NBTTagCompound.h"
#include "PathEntity.h"
#include "Random.h"
// HANDWRITTEN OVERRIDE: dropped bogus `#include "Set.h"` / `#include "std::set<int>.h"`.
// java.util.Set/TreeSet -> std::vector (see World.h). NextTickListEntry needed for the
// scheduled-tick vectors.
#include "NextTickListEntry.h"
#include "TileEntity.h"
#include <vector>
#include "Vec3D.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include <algorithm>
#include <string>
#include "Block.h"
#include "Pathfinder.h"
#include "BlockFluids.h"
#include "ChunkCache.h"
#include "ChunkCoordIntPair.h"
#include "ChunkProviderLoadOrGenerate.h"
#include "SpawnerAnimals.h"
#include "WorldProviderHell.h"
#include "MetadataChunkBlock.h"
#include "MobSpawnerBase.h"
#include <float.h>

// HANDWRITTEN OVERRIDE helpers: java.lang.Double.isNaN / isInfinite (no Double class on 360).
static bool _dIsNaN(double v) { return _isnan(v) != 0; }
static bool _dIsInfinite(double v) { return _finite(v) == 0 && _isnan(v) == 0; }

// HANDWRITTEN OVERRIDE helper: Java List.removeAll(other) over std::vector<void*>.
static void _vecRemoveAllVoid(std::vector<void*>& list, std::vector<void*>& toRemove) {
	for(size_t _r = 0; _r < toRemove.size(); ++_r) {
		void* _val = toRemove[_r];
		for(size_t _i = 0; _i < list.size(); ) {
			if(list[_i] == _val) {
				list.erase(list.begin() + _i);
			} else {
				++_i;
			}
		}
	}
}

int World::s_field_9429_y = 0;

World::World()
	: m_field_4214_a(false)
	, m_scheduledTickTreeSet(NULL)
	, m_scheduledTickSet(NULL)
	, m_worldTime(0)
	, m_field_1019_F(0)
	, m_skylightSubtracted(0)
	, m_field_9437_g(0)
	, m_field_9436_h(0)
	, m_field_1043_h(false)
	, m_field_1054_E(0)
	, m_autosavePeriod(0)
	, m_difficultySetting(0)
	, rand(NULL)
	, m_spawnX(0)
	, m_spawnY(0)
	, m_spawnZ(0)
	, m_field_1033_r(false)
	, m_worldProvider(NULL)
	, m_chunkProvider(NULL)
	, m_field_9433_s(NULL)
	, m_field_9432_t(NULL)
	, m_randomSeed(0)
	, m_nbtCompoundPlayer(NULL)
	, m_sizeOnDisk(0)
	, m_field_9431_w("")
	, m_field_9430_x(false)
	, m_field_4204_J(0)
	, m_field_9427_K(NULL)
	, m_field_9426_L(0)
	, m_multiplayerWorld(false)
{
}

World::World(std::string /* PORT: was java.io.File */* var1, const std::string& var2)
	: m_field_4214_a(false)
	, m_scheduledTickTreeSet(NULL)
	, m_scheduledTickSet(NULL)
	, m_worldTime(0)
	, m_field_1019_F(0)
	, m_skylightSubtracted(0)
	, m_field_9437_g(0)
	, m_field_9436_h(0)
	, m_field_1043_h(false)
	, m_field_1054_E(0)
	, m_autosavePeriod(0)
	, m_difficultySetting(0)
	, rand(NULL)
	, m_spawnX(0)
	, m_spawnY(0)
	, m_spawnZ(0)
	, m_field_1033_r(false)
	, m_worldProvider(NULL)
	, m_chunkProvider(NULL)
	, m_field_9433_s(NULL)
	, m_field_9432_t(NULL)
	, m_randomSeed(0)
	, m_nbtCompoundPlayer(NULL)
	, m_sizeOnDisk(0)
	, m_field_9431_w("")
	, m_field_9430_x(false)
	, m_field_4204_J(0)
	, m_field_9427_K(NULL)
	, m_field_9426_L(0)
	, m_multiplayerWorld(false)
{
  // PORT: constructor delegation  // was: this(var1, var2, (new Random())->nextLong());
}

World::World(const std::string& var1, WorldProvider* var2, long long var3)
	: m_field_4214_a(false)
	, m_scheduledTickTreeSet(NULL)
	, m_scheduledTickSet(NULL)
	, m_worldTime(0)
	, m_field_1019_F(0)
	, m_skylightSubtracted(0)
	, m_field_9437_g(0)
	, m_field_9436_h(0)
	, m_field_1043_h(false)
	, m_field_1054_E(0)
	, m_autosavePeriod(0)
	, m_difficultySetting(0)
	, rand(NULL)
	, m_spawnX(0)
	, m_spawnY(0)
	, m_spawnZ(0)
	, m_field_1033_r(false)
	, m_worldProvider(NULL)
	, m_chunkProvider(NULL)
	, m_field_9433_s(NULL)
	, m_field_9432_t(NULL)
	, m_randomSeed(0)
	, m_nbtCompoundPlayer(NULL)
	, m_sizeOnDisk(0)
	, m_field_9431_w("")
	, m_field_9430_x(false)
	, m_field_4204_J(0)
	, m_field_9427_K(NULL)
	, m_field_9426_L(0)
	, m_multiplayerWorld(false)
{
		m_field_4214_a = false;
		m_field_1051_z.clear();
		m_loadedEntityList.clear();
		m_field_1024_A.clear();
		m_scheduledTickTreeSet = new std::vector<NextTickListEntry*>();  // was TreeSet
		m_scheduledTickSet = new std::vector<NextTickListEntry*>();      // was HashSet
		m_loadedTileEntityList.clear();
		m_playerEntities.clear();
		m_worldTime = 0L;
		m_field_1019_F = 16777215L;
		m_skylightSubtracted = 0;
		m_field_9437_g = (new Random())->nextInt();
		m_field_9436_h = 1013904223;
		m_field_1043_h = false;
		m_field_1054_E = GetTickCount();
		m_autosavePeriod = 40;
		rand = new Random();
		m_field_1033_r = false;
		m_worldAccesses.clear();
		m_randomSeed = 0L;
		m_sizeOnDisk = 0L;
		m_field_9428_I.clear();
		m_field_4204_J = 0;
		m_field_9427_K = new std::vector<void*>();  // was HashSet
		m_field_9426_L = rand->nextInt(12000);
		m_field_1012_M.clear();
		m_multiplayerWorld = false;
		m_field_9431_w = var1;
		m_randomSeed = var3;
		m_worldProvider = var2;
		var2->registerWorld(this);
		m_chunkProvider = func_4081_a(m_field_9432_t);
		calculateInitialSkylight();
}

World::World(World* var1, WorldProvider* var2)
	: m_field_4214_a(false)
	, m_scheduledTickTreeSet(NULL)
	, m_scheduledTickSet(NULL)
	, m_worldTime(0)
	, m_field_1019_F(0)
	, m_skylightSubtracted(0)
	, m_field_9437_g(0)
	, m_field_9436_h(0)
	, m_field_1043_h(false)
	, m_field_1054_E(0)
	, m_autosavePeriod(0)
	, m_difficultySetting(0)
	, rand(NULL)
	, m_spawnX(0)
	, m_spawnY(0)
	, m_spawnZ(0)
	, m_field_1033_r(false)
	, m_worldProvider(NULL)
	, m_chunkProvider(NULL)
	, m_field_9433_s(NULL)
	, m_field_9432_t(NULL)
	, m_randomSeed(0)
	, m_nbtCompoundPlayer(NULL)
	, m_sizeOnDisk(0)
	, m_field_9431_w("")
	, m_field_9430_x(false)
	, m_field_4204_J(0)
	, m_field_9427_K(NULL)
	, m_field_9426_L(0)
	, m_multiplayerWorld(false)
{
		m_field_4214_a = false;
		m_field_1051_z.clear();
		m_loadedEntityList.clear();
		m_field_1024_A.clear();
		m_scheduledTickTreeSet = new std::vector<NextTickListEntry*>();  // was TreeSet
		m_scheduledTickSet = new std::vector<NextTickListEntry*>();      // was HashSet
		m_loadedTileEntityList.clear();
		m_playerEntities.clear();
		m_worldTime = 0L;
		m_field_1019_F = 16777215L;
		m_skylightSubtracted = 0;
		m_field_9437_g = (new Random())->nextInt();
		m_field_9436_h = 1013904223;
		m_field_1043_h = false;
		m_field_1054_E = GetTickCount();
		m_autosavePeriod = 40;
		rand = new Random();
		m_field_1033_r = false;
		m_worldAccesses.clear();
		m_randomSeed = 0L;
		m_sizeOnDisk = 0L;
		m_field_9428_I.clear();
		m_field_4204_J = 0;
		m_field_9427_K = new std::vector<void*>();  // was HashSet
		m_field_9426_L = rand->nextInt(12000);
		m_field_1012_M.clear();
		m_multiplayerWorld = false;
		m_field_1054_E = var1->m_field_1054_E;
		m_field_9433_s = var1->m_field_9433_s;
		m_field_9432_t = var1->m_field_9432_t;
		m_field_9431_w = var1->m_field_9431_w;
		m_randomSeed = var1->m_randomSeed;
		m_worldTime = var1->m_worldTime;
		m_spawnX = var1->m_spawnX;
		m_spawnY = var1->m_spawnY;
		m_spawnZ = var1->m_spawnZ;
		m_sizeOnDisk = var1->m_sizeOnDisk;
		m_worldProvider = var2;
		var2->registerWorld(this);
		m_chunkProvider = func_4081_a(m_field_9432_t);
		calculateInitialSkylight();
}

World::World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3)
	: m_field_4214_a(false)
	, m_scheduledTickTreeSet(NULL)
	, m_scheduledTickSet(NULL)
	, m_worldTime(0)
	, m_field_1019_F(0)
	, m_skylightSubtracted(0)
	, m_field_9437_g(0)
	, m_field_9436_h(0)
	, m_field_1043_h(false)
	, m_field_1054_E(0)
	, m_autosavePeriod(0)
	, m_difficultySetting(0)
	, rand(NULL)
	, m_spawnX(0)
	, m_spawnY(0)
	, m_spawnZ(0)
	, m_field_1033_r(false)
	, m_worldProvider(NULL)
	, m_chunkProvider(NULL)
	, m_field_9433_s(NULL)
	, m_field_9432_t(NULL)
	, m_randomSeed(0)
	, m_nbtCompoundPlayer(NULL)
	, m_sizeOnDisk(0)
	, m_field_9431_w("")
	, m_field_9430_x(false)
	, m_field_4204_J(0)
	, m_field_9427_K(NULL)
	, m_field_9426_L(0)
	, m_multiplayerWorld(false)
{
  // PORT: constructor delegation  // was: this(var1, var2, var3, NULL);
}

World::World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3, WorldProvider* var5)
	: m_field_4214_a(false)
	, m_scheduledTickTreeSet(NULL)
	, m_scheduledTickSet(NULL)
	, m_worldTime(0)
	, m_field_1019_F(0)
	, m_skylightSubtracted(0)
	, m_field_9437_g(0)
	, m_field_9436_h(0)
	, m_field_1043_h(false)
	, m_field_1054_E(0)
	, m_autosavePeriod(0)
	, m_difficultySetting(0)
	, rand(NULL)
	, m_spawnX(0)
	, m_spawnY(0)
	, m_spawnZ(0)
	, m_field_1033_r(false)
	, m_worldProvider(NULL)
	, m_chunkProvider(NULL)
	, m_field_9433_s(NULL)
	, m_field_9432_t(NULL)
	, m_randomSeed(0)
	, m_nbtCompoundPlayer(NULL)
	, m_sizeOnDisk(0)
	, m_field_9431_w("")
	, m_field_9430_x(false)
	, m_field_4204_J(0)
	, m_field_9427_K(NULL)
	, m_field_9426_L(0)
	, m_multiplayerWorld(false)
{
		m_field_4214_a = false;
		m_field_1051_z.clear();
		m_loadedEntityList.clear();
		m_field_1024_A.clear();
		m_scheduledTickTreeSet = new std::vector<NextTickListEntry*>();  // was TreeSet
		m_scheduledTickSet = new std::vector<NextTickListEntry*>();      // was HashSet
		m_loadedTileEntityList.clear();
		m_playerEntities.clear();
		m_worldTime = 0L;
		m_field_1019_F = 16777215L;
		m_skylightSubtracted = 0;
		m_field_9437_g = (new Random())->nextInt();
		m_field_9436_h = 1013904223;
		m_field_1043_h = false;
		m_field_1054_E = GetTickCount();
		m_autosavePeriod = 40;
		rand = new Random();
		m_field_1033_r = false;
		m_worldAccesses.clear();
		m_randomSeed = 0L;
		m_sizeOnDisk = 0L;
		m_field_9428_I.clear();
		m_field_4204_J = 0;
		m_field_9427_K = new std::vector<void*>();  // was HashSet
		m_field_9426_L = rand->nextInt(12000);
		m_field_1012_M.clear();
		m_multiplayerWorld = false;
		m_field_9433_s = var1;
		m_field_9431_w = var2;
		// PORT: java.io.File world-dir creation + session.lock write + level.dat NBT load are
		// handled by the STFS save path (ContentStorage / Save360); the converter's java.io.File
		// fields here are std::string* placeholders. Treat as a fresh world (m_field_1033_r=true);
		// ContentStorage performs the real container mount + level.dat load/save.
		m_field_9432_t = NULL;  // (was: new File(var1, var2)) -- std::string* File-path placeholder; STFS path owns the real dir

		void* var17 = new WorldProvider();
		m_field_1033_r = true;  // (was: !new File(m_field_9432_t,"level.dat").exists())
		if(false) {  // PORT: NBT level.dat load done by ContentStorage, not java.io.File here
		}

		if(var5 != NULL) {
			var17 = var5;
		}

		bool var19 = false;
		if(m_randomSeed == 0L) {
			m_randomSeed = var3;
			var19 = true;
		}

		m_worldProvider = (WorldProvider*)var17;
		m_worldProvider->registerWorld(this);
		m_chunkProvider = func_4081_a(m_field_9432_t);
		if(var19) {
			m_field_9430_x = true;
			m_spawnX = 0;
			m_spawnY = 64;

			for(m_spawnZ = 0; !m_worldProvider->canCoordinateBeSpawn(m_spawnX, m_spawnZ); m_spawnZ += rand->nextInt(64) - rand->nextInt(64)) {
				m_spawnX += rand->nextInt(64) - rand->nextInt(64);
			}

			m_field_9430_x = false;
		}

		calculateInitialSkylight();
}

World::~World() {
	m_scheduledTickTreeSet = NULL;
	m_scheduledTickSet = NULL;
	rand = NULL;
	m_worldProvider = NULL;
	m_chunkProvider = NULL;
	m_field_9433_s = NULL;
	m_field_9432_t = NULL;
	m_nbtCompoundPlayer = NULL;
	m_field_9427_K = NULL;
}

NBTTagCompound* World::func_629_a(std::string /* PORT: was java.io.File */* var0, const std::string& var1) {
		// PORT: reading saves/<name>/level.dat is handled by the STFS save path (ContentStorage).
		// The converter's java.io.File chain (File/exists/FileInputStream NBT) is replaced there.
		(void)var0; (void)var1;
		return NULL;
}

void World::deleteWorld(std::string /* PORT: was java.io.File */* var0, const std::string& var1) {
		// PORT: world deletion goes through the STFS container API (ContentStorage / Save360),
		// not java.io.File listFiles()/delete().
		(void)var0; (void)var1;
}

void World::deleteFiles(std::string /* PORT: was java.io.File */* var0) {
		// PORT: recursive java.io.File delete -> handled by the STFS container API.
		(void)var0;
}

WorldChunkManager* World::func_4075_a() {
		return m_worldProvider->m_worldChunkMgr;
}

IChunkProvider* World::func_4081_a(std::string /* PORT: was java.io.File */* var1) {
		return new ChunkProviderLoadOrGenerate(this, m_worldProvider->getChunkLoader(var1), m_worldProvider->getChunkProvider());
}

void World::func_4076_b() {
		if(m_spawnY <= 0) {
			m_spawnY = 64;
		}

		while(func_614_g(m_spawnX, m_spawnZ) == 0) {
			m_spawnX += rand->nextInt(8) - rand->nextInt(8);
			m_spawnZ += rand->nextInt(8) - rand->nextInt(8);
		}

}

int World::func_614_g(int var1, int var2) {
		size_t var3;
		for(var3 = 63; getBlockId(var1, var3 + 1, var2) != 0; ++var3) {
		}

		return getBlockId(var1, var3, var2);
}

void World::func_6464_c() {
	// TODO: implement
}

void World::func_608_a(EntityPlayer* var1) {
  { // was try
			if(m_nbtCompoundPlayer != NULL) {
				var1->readFromNBT(m_nbtCompoundPlayer);
				m_nbtCompoundPlayer = NULL;
			}

			entityJoinedWorld(var1);
		} if(false) { // was catch(Exception var3)
   // Removed Java stream/exception call: var3.printStackTrace();
		}

}

void World::saveWorld(bool var1, IProgressUpdate* var2) {
		if(m_chunkProvider->func_536_b()) {
			if(var2 != NULL) {
				var2->func_594_b("Saving level");
			}

			saveLevel();
			if(var2 != NULL) {
				var2->displayLoadingString("Saving chunks");
			}

			m_chunkProvider->saveChunks(var1, var2);
		}
}

void World::saveLevel() {
		func_663_l();
		NBTTagCompound* var1 = new NBTTagCompound();
		var1->setLong("RandomSeed", m_randomSeed);
		var1->setInteger("SpawnX", m_spawnX);
		var1->setInteger("SpawnY", m_spawnY);
		var1->setInteger("SpawnZ", m_spawnZ);
		var1->setLong("Time", m_worldTime);
		var1->setLong("SizeOnDisk", m_sizeOnDisk);
		var1->setLong("LastPlayed", GetTickCount());
		EntityPlayer* var2 = NULL;
		if(m_playerEntities.size() > 0) {
			var2 = (EntityPlayer*)m_playerEntities[0];
		}

		NBTTagCompound* var3 = NULL;
		if(var2 != NULL) {
			var3 = new NBTTagCompound();
			var2->writeToNBT(var3);
			var1->setCompoundTag("Player", var3);
		}

		var3 = new NBTTagCompound();
		var3->setTag("Data", var1);

		// PORT: the level.dat write (+ .dat_new/.dat_old rotation) is performed by the STFS save
		// path (ContentStorage -> CompressedStreamTools to "Save:\\level.dat"). The Data compound
		// (var3) is built above as in vanilla; persisting it is delegated there.
		(void)var3;

}

bool World::func_650_a(int var1) {
		if(!m_chunkProvider->func_536_b()) {
			return true;
		} else {
			if(var1 == 0) {
				saveLevel();
			}

			return m_chunkProvider->saveChunks(false, NULL);
		}
}

int World::getBlockId(int var1, int var2, int var3) {
		return var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000 ? (var2 < 0 ? 0 : (var2 >= 128 ? 0 : getChunkFromChunkCoords(var1 >> 4, var3 >> 4)->getBlockID(var1 & 15, var2, var3 & 15))) : 0;
}

bool World::blockExists(int var1, int var2, int var3) {
		return var2 >= 0 && var2 < 128 ? chunkExists(var1 >> 4, var3 >> 4) : false;
}

bool World::checkChunksExist(int var1, int var2, int var3, int var4, int var5, int var6) {
		if(var5 >= 0 && var2 < 128) {
			var1 >>= 4;
			var2 >>= 4;
			var3 >>= 4;
			var4 >>= 4;
			var5 >>= 4;
			var6 >>= 4;

			for(int var7 = var1; var7 <= var4; ++var7) {
				for(int var8 = var3; var8 <= var6; ++var8) {
					if(!chunkExists(var7, var8)) {
						return false;
					}
				}
			}

			return true;
		} else {
			return false;
		}
}

bool World::chunkExists(int var1, int var2) {
		return m_chunkProvider->chunkExists(var1, var2);
}

Chunk* World::getChunkFromBlockCoords(int var1, int var2) {
		return getChunkFromChunkCoords(var1 >> 4, var2 >> 4);
}

Chunk* World::getChunkFromChunkCoords(int var1, int var2) {
		return m_chunkProvider->provideChunk(var1, var2);
}

bool World::setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			if(var2 < 0) {
				return false;
			} else if(var2 >= 128) {
				return false;
			} else {
				Chunk* var6 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
				return var6->setBlockIDWithMetadata(var1 & 15, var2, var3 & 15, var4, var5);
			}
		} else {
			return false;
		}
}

bool World::setBlock(int var1, int var2, int var3, int var4) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			if(var2 < 0) {
				return false;
			} else if(var2 >= 128) {
				return false;
			} else {
				Chunk* var5 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
				return var5->setBlockID(var1 & 15, var2, var3 & 15, var4);
			}
		} else {
			return false;
		}
}

Material* World::getBlockMaterial(int var1, int var2, int var3) {
		int var4 = getBlockId(var1, var2, var3);
		return var4 == 0 ? Material::s_air : Block::blocksList[var4]->m_blockMaterial;
}

int World::getBlockMetadata(int var1, int var2, int var3) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			if(var2 < 0) {
				return 0;
			} else if(var2 >= 128) {
				return 0;
			} else {
				Chunk* var4 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
				var1 &= 15;
				var3 &= 15;
				return var4->getBlockMetadata(var1, var2, var3);
			}
		} else {
			return 0;
		}
}

void World::setBlockMetadataWithNotify(int var1, int var2, int var3, int var4) {
		if(setBlockMetadata(var1, var2, var3, var4)) {
			notifyBlockChange(var1, var2, var3, getBlockId(var1, var2, var3));
		}

}

bool World::setBlockMetadata(int var1, int var2, int var3, int var4) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			if(var2 < 0) {
				return false;
			} else if(var2 >= 128) {
				return false;
			} else {
				Chunk* var5 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
				var1 &= 15;
				var3 &= 15;
				var5->setBlockMetadata(var1, var2, var3, var4);
				return true;
			}
		} else {
			return false;
		}
}

bool World::setBlockWithNotify(int var1, int var2, int var3, int var4) {
		if(setBlock(var1, var2, var3, var4)) {
			notifyBlockChange(var1, var2, var3, var4);
			return true;
		} else {
			return false;
		}
}

bool World::setBlockAndMetadataWithNotify(int var1, int var2, int var3, int var4, int var5) {
		if(setBlockAndMetadata(var1, var2, var3, var4, var5)) {
			notifyBlockChange(var1, var2, var3, var4);
			return true;
		} else {
			return false;
		}
}

void World::func_665_h(int var1, int var2, int var3) {
		for(size_t var4 = 0; var4 < m_worldAccesses.size(); ++var4) {
			m_worldAccesses[var4]->func_934_a(var1, var2, var3);
		}

}

void World::notifyBlockChange(int var1, int var2, int var3, int var4) {
		func_665_h(var1, var2, var3);
		notifyBlocksOfNeighborChange(var1, var2, var3, var4);
}

void World::func_680_f(int var1, int var2, int var3, int var4) {
		if(var3 > var4) {
			int var5 = var4;
			var4 = var3;
			var3 = var5;
		}

		func_701_b(var1, var3, var2, var1, var4, var2);
}

void World::func_701_b(int var1, int var2, int var3, int var4, int var5, int var6) {
		for(size_t var7 = 0; var7 < m_worldAccesses.size(); ++var7) {
			m_worldAccesses[var7]->func_937_b(var1, var2, var3, var4, var5, var6);
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

void World::notifyBlockOfNeighborChange(int var1, int var2, int var3, int var4) {
		if(!m_field_1043_h && !m_multiplayerWorld) {
			Block* var5 = Block::blocksList[getBlockId(var1, var2, var3)];
			if(var5 != NULL) {
				var5->onNeighborBlockChange(this, var1, var2, var3, var4);
			}

		}
}

bool World::canBlockSeeTheSky(int var1, int var2, int var3) {
		return getChunkFromChunkCoords(var1 >> 4, var3 >> 4)->canBlockSeeTheSky(var1 & 15, var2, var3 & 15);
}

int World::getBlockLightValue(int var1, int var2, int var3) {
		return getBlockLightValue(var1, var2, var3, true);
}

int World::getBlockLightValue(int var1, int var2, int var3, bool var4) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			size_t var5;
			if(var4) {
				var5 = getBlockId(var1, var2, var3);
				if(var5 == Block::s_stairSingle->blockID || var5 == Block::s_tilledField->blockID) {
					int var6 = getBlockLightValue(var1, var2 + 1, var3, false);
					int var7 = getBlockLightValue(var1 + 1, var2, var3, false);
					int var8 = getBlockLightValue(var1 - 1, var2, var3, false);
					int var9 = getBlockLightValue(var1, var2, var3 + 1, false);
					int var10 = getBlockLightValue(var1, var2, var3 - 1, false);
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
				var5 = 15 - m_skylightSubtracted;
				if(var5 < 0) {
					var5 = 0;
				}

				return var5;
			} else {
				Chunk* var11 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
				var1 &= 15;
				var3 &= 15;
				return var11->getBlockLightValue(var1, var2, var3, m_skylightSubtracted);
			}
		} else {
			return 15;
		}
}

bool World::canExistingBlockSeeTheSky(int var1, int var2, int var3) {
		if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
			if(var2 < 0) {
				return false;
			} else if(var2 >= 128) {
				return true;
			} else if(!chunkExists(var1 >> 4, var3 >> 4)) {
				return false;
			} else {
				Chunk* var4 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
				var1 &= 15;
				var3 &= 15;
				return var4->canBlockSeeTheSky(var1, var2, var3);
			}
		} else {
			return false;
		}
}

int World::getHeightValue(int var1, int var2) {
		if(var1 >= -32000000 && var2 >= -32000000 && var1 < 32000000 && var2 <= 32000000) {
			if(!chunkExists(var1 >> 4, var2 >> 4)) {
				return 0;
			} else {
				Chunk* var3 = getChunkFromChunkCoords(var1 >> 4, var2 >> 4);
				return var3->getHeightValue(var1 & 15, var2 & 15);
			}
		} else {
			return 0;
		}
}

void World::neighborLightPropagationChanged(EnumSkyBlock* var1, int var2, int var3, int var4, int var5) {
		if(!m_worldProvider->m_field_6478_e || var1 != EnumSkyBlock::s_Sky) {
			if(blockExists(var2, var3, var4)) {
				if(var1 == EnumSkyBlock::s_Sky) {
					if(canExistingBlockSeeTheSky(var2, var3, var4)) {
						var5 = 15;
					}
				} else if(var1 == EnumSkyBlock::s_Block) {
					int var6 = getBlockId(var2, var3, var4);
					if(Block::s_lightValue[var6] > var5) {
						var5 = Block::s_lightValue[var6];
					}
				}

				if(getSavedLightValue(var1, var2, var3, var4) != var5) {
					func_616_a(var1, var2, var3, var4, var2, var3, var4);
				}

			}
		}
}

int World::getSavedLightValue(EnumSkyBlock* var1, int var2, int var3, int var4) {
		if(var3 >= 0 && var3 < 128 && var2 >= -32000000 && var4 >= -32000000 && var2 < 32000000 && var4 <= 32000000) {
			int var5 = var2 >> 4;
			int var6 = var4 >> 4;
			if(!chunkExists(var5, var6)) {
				return 0;
			} else {
				Chunk* var7 = getChunkFromChunkCoords(var5, var6);
				return var7->getSavedLightValue(var1, var2 & 15, var3, var4 & 15);
			}
		} else {
			return var1->field_1722_c;
		}
}

void World::setLightValue(EnumSkyBlock* var1, int var2, int var3, int var4, int var5) {
		if(var2 >= -32000000 && var4 >= -32000000 && var2 < 32000000 && var4 <= 32000000) {
			if(var3 >= 0) {
				if(var3 < 128) {
					if(chunkExists(var2 >> 4, var4 >> 4)) {
						Chunk* var6 = getChunkFromChunkCoords(var2 >> 4, var4 >> 4);
						var6->setLightValue(var1, var2 & 15, var3, var4 & 15, var5);

						for(size_t var7 = 0; var7 < m_worldAccesses.size(); ++var7) {
							m_worldAccesses[var7]->func_934_a(var2, var3, var4);
						}

					}
				}
			}
		}
}

float World::getLightBrightness(int var1, int var2, int var3) {
		return m_worldProvider->m_lightBrightnessTable[getBlockLightValue(var1, var2, var3)];
}

bool World::isDaytime() {
		return m_skylightSubtracted < 4;
}

MovingObjectPosition* World::rayTraceBlocks(Vec3D* var1, Vec3D* var2) {
		return rayTraceBlocks(var1, var2, false);
}

MovingObjectPosition* World::rayTraceBlocks(Vec3D* var1, Vec3D* var2, bool var3) {
		if(!((var1->xCoord) != (var1->xCoord)) && !((var1->yCoord) != (var1->yCoord)) && !((var1->zCoord) != (var1->zCoord))) {
			if(!((var2->xCoord) != (var2->xCoord)) && !((var2->yCoord) != (var2->yCoord)) && !((var2->zCoord) != (var2->zCoord))) {
				int var4 = (int)floor(var2->xCoord);
				int var5 = (int)floor(var2->yCoord);
				int var6 = (int)floor(var2->zCoord);
				int var7 = (int)floor(var1->xCoord);
				int var8 = (int)floor(var1->yCoord);
				int var9 = (int)floor(var1->zCoord);
				int var10 = 200;

				while(var10-- >= 0) {
					if(((var1->xCoord) != (var1->xCoord)) || ((var1->yCoord) != (var1->yCoord)) || ((var1->zCoord) != (var1->zCoord))) {
						return NULL;
					}

					if(var7 == var4 && var8 == var5 && var9 == var6) {
						return NULL;
					}

					double var11 = 999.0f;
					double var13 = 999.0f;
					double var15 = 999.0f;
					if(var4 > var7) {
						var11 = (double)var7 + 1.0f;
					}

					if(var4 < var7) {
						var11 = (double)var7 + 0.0f;
					}

					if(var5 > var8) {
						var13 = (double)var8 + 1.0f;
					}

					if(var5 < var8) {
						var13 = (double)var8 + 0.0f;
					}

					if(var6 > var9) {
						var15 = (double)var9 + 1.0f;
					}

					if(var6 < var9) {
						var15 = (double)var9 + 0.0f;
					}

					double var17 = 999.0f;
					double var19 = 999.0f;
					double var21 = 999.0f;
					double var23 = var2->xCoord - var1->xCoord;
					double var25 = var2->yCoord - var1->yCoord;
					double var27 = var2->zCoord - var1->zCoord;
					if(var11 != 999.0f) {
						var17 = (var11 - var1->xCoord) / var23;
					}

					if(var13 != 999.0f) {
						var19 = (var13 - var1->yCoord) / var25;
					}

					if(var15 != 999.0f) {
						var21 = (var15 - var1->zCoord) / var27;
					}

					bool var29 = false;
					unsigned char var35;
					if(var17 < var19 && var17 < var21) {
						if(var4 > var7) {
							var35 = 4;
						} else {
							var35 = 5;
						}

						var1->xCoord = var11;
						var1->yCoord += var25 * var17;
						var1->zCoord += var27 * var17;
					} else if(var19 < var21) {
						if(var5 > var8) {
							var35 = 0;
						} else {
							var35 = 1;
						}

						var1->xCoord += var23 * var19;
						var1->yCoord = var13;
						var1->zCoord += var27 * var19;
					} else {
						if(var6 > var9) {
							var35 = 2;
						} else {
							var35 = 3;
						}

						var1->xCoord += var23 * var21;
						var1->yCoord += var25 * var21;
						var1->zCoord = var15;
					}

					Vec3D* var30 = Vec3D::createVector(var1->xCoord, var1->yCoord, var1->zCoord);
					var7 = (int)(var30->xCoord = (double)(int)floor(var1->xCoord));
					if(var35 == 5) {
						--var7;
						++var30->xCoord;
					}

					var8 = (int)(var30->yCoord = (double)(int)floor(var1->yCoord));
					if(var35 == 1) {
						--var8;
						++var30->yCoord;
					}

					var9 = (int)(var30->zCoord = (double)(int)floor(var1->zCoord));
					if(var35 == 3) {
						--var9;
						++var30->zCoord;
					}

					int var31 = getBlockId(var7, var8, var9);
					int var32 = getBlockMetadata(var7, var8, var9);
					Block* var33 = Block::blocksList[var31];
					if(var31 > 0 && var33->canCollideCheck(var32, var3)) {
						MovingObjectPosition* var34 = var33->collisionRayTrace(this, var7, var8, var9, var1, var2);
						if(var34 != NULL) {
							return var34;
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

void World::playSoundAtEntity(Entity* var1, const std::string& var2, float var3, float var4) {
		for(size_t var5 = 0; var5 < m_worldAccesses.size(); ++var5) {
			m_worldAccesses[var5]->playSound(var2, var1->posX, var1->posY - (double)var1->yOffset, var1->posZ, var3, var4);
		}

}

void World::playSoundEffect(double var1, double var3, double var5, const std::string& var7, float var8, float var9) {
		for(size_t var10 = 0; var10 < m_worldAccesses.size(); ++var10) {
			m_worldAccesses[var10]->playSound(var7, var1, var3, var5, var8, var9);
		}

}

void World::playRecord(const std::string& var1, int var2, int var3, int var4) {
		for(size_t var5 = 0; var5 < m_worldAccesses.size(); ++var5) {
			m_worldAccesses[var5]->playRecord(var1, var2, var3, var4);
		}

}

void World::spawnParticle(const std::string& var1, double var2, double var4, double var6, double var8, double var10, double var12) {
		for(size_t var14 = 0; var14 < m_worldAccesses.size(); ++var14) {
			m_worldAccesses[var14]->spawnParticle(var1, var2, var4, var6, var8, var10, var12);
		}

}

bool World::entityJoinedWorld(Entity* var1) {
		int var2 = (int)floor(var1->posX / 16.0f);
		int var3 = (int)floor(var1->posZ / 16.0f);
		bool var4 = false;
		if(dynamic_cast<EntityPlayer*>(var1) != NULL) {
			var4 = true;
		}

		if(!var4 && !chunkExists(var2, var3)) {
			return false;
		} else {
			if(dynamic_cast<EntityPlayer*>(var1) != NULL) {
				m_playerEntities.push_back((EntityPlayer*)var1);
				DbgPrintF("%s\n", "Player count: " + m_playerEntities.size());
			}

			getChunkFromChunkCoords(var2, var3)->addEntity(var1);
			m_loadedEntityList.push_back(var1);
			obtainEntitySkin(var1);
			return true;
		}
}

void World::obtainEntitySkin(Entity* var1) {
		for(size_t var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
			m_worldAccesses[var2]->obtainEntitySkin(var1);
		}

}

void World::releaseEntitySkin(Entity* var1) {
		for(size_t var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
			m_worldAccesses[var2]->releaseEntitySkin(var1);
		}

}

void World::setEntityDead(Entity* var1) {
		var1->setEntityDead();
		if(dynamic_cast<EntityPlayer*>(var1) != NULL) {
			_vecRemoveObj(m_playerEntities, (EntityPlayer*)var1);
		}

}

void World::addWorldAccess(IWorldAccess* var1) {
		m_worldAccesses.push_back(var1);
}

void World::removeWorldAccess(IWorldAccess* var1) {
  std::vector<IWorldAccess*>::iterator _removeIt = std::find(m_worldAccesses.begin(), m_worldAccesses.end(), var1);
  if (_removeIt != m_worldAccesses.end()) m_worldAccesses.erase(_removeIt);
}

std::vector<AxisAlignedBB> World::getCollidingBoundingBoxes(Entity* var1, AxisAlignedBB var2) {
  // HANDWRITTEN OVERRIDE: collect AABB VALUES in a typed local (the void* field m_field_9428_I
  // matches Block::getCollidingBoundingBoxes' (void*) param and is kept for that call only).
  std::vector<AxisAlignedBB> _result;
  m_field_9428_I.clear();
		int var3 = (int)floor(var2.minX);
		int var4 = (int)floor(var2.maxX + 1.0f);
		int var5 = (int)floor(var2.minY);
		int var6 = (int)floor(var2.maxY + 1.0f);
		int var7 = (int)floor(var2.minZ);
		int var8 = (int)floor(var2.maxZ + 1.0f);

		for(int var9 = var3; var9 < var4; ++var9) {
			for(int var10 = var7; var10 < var8; ++var10) {
				if(blockExists(var9, 64, var10)) {
					for(int var11 = var5 - 1; var11 < var6; ++var11) {
						Block* var12 = Block::blocksList[getBlockId(var9, var11, var10)];
						if(var12 != NULL) {
							var12->getCollidingBoundingBoxes(this, var9, var11, var10, var2, m_field_9428_I);
						}
					}
				}
			}
		}

		double var14 = 0.25f;
		std::vector<void*> var15 = getEntitiesWithinAABBExcludingEntity(var1, var2.expands(var14, var14, var14));

		for(size_t var16 = 0; var16 < var15.size(); ++var16) {
			AxisAlignedBB var13 = ((Entity*)var15[var16])->func_372_f_();
			if(true && var13.intersectsWith(var2)) {
				_result.push_back(var13);
			}

			var13 = var1->func_383_b_((Entity*)var15[var16]);
			if(true && var13.intersectsWith(var2)) {
				_result.push_back(var13);
			}
		}

		return _result;
}

int World::calculateSkylightSubtracted(float var1) {
		float var2 = getCelestialAngle(var1);
		float var3 = 1.0F - (cosf(var2 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F);
		if(var3 < 0.0F) {
			var3 = 0.0F;
		}

		if(var3 > 1.0F) {
			var3 = 1.0F;
		}

		return (int)(var3 * 11.0F);
}

Vec3D* World::func_4079_a(Entity* var1, float var2) {
		float var3 = getCelestialAngle(var2);
		float var4 = cosf(var3 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F;
		if(var4 < 0.0F) {
			var4 = 0.0F;
		}

		if(var4 > 1.0F) {
			var4 = 1.0F;
		}

		int var5 = (int)floor(var1->posX);
		int var6 = (int)floor(var1->posZ);
		float var7 = (float)func_4075_a()->func_4072_b(var5, var6);
		int var8 = func_4075_a()->func_4073_a(var5, var6)->getSkyColorByTemp(var7);
		float var9 = (float)(var8 >> 16 & 255) / 255.0F;
		float var10 = (float)(var8 >> 8 & 255) / 255.0F;
		float var11 = (float)(var8 & 255) / 255.0F;
		var9 *= var4;
		var10 *= var4;
		var11 *= var4;
		return Vec3D::createVector((double)var9, (double)var10, (double)var11);
}

float World::getCelestialAngle(float var1) {
		return m_worldProvider->calculateCelestialAngle(m_worldTime, var1);
}

Vec3D* World::func_628_d(float var1) {
		float var2 = getCelestialAngle(var1);
		float var3 = cosf(var2 * 3.14159265358979323846f * 2.0F) * 2.0F + 0.5F;
		if(var3 < 0.0F) {
			var3 = 0.0F;
		}

		if(var3 > 1.0F) {
			var3 = 1.0F;
		}

		float var4 = (float)(m_field_1019_F >> 16 & 255L) / 255.0F;
		float var5 = (float)(m_field_1019_F >> 8 & 255L) / 255.0F;
		float var6 = (float)(m_field_1019_F & 255L) / 255.0F;
		var4 *= var3 * 0.9F + 0.1F;
		var5 *= var3 * 0.9F + 0.1F;
		var6 *= var3 * 0.85F + 0.15F;
		return Vec3D::createVector((double)var4, (double)var5, (double)var6);
}

Vec3D* World::func_4082_d(float var1) {
		float var2 = getCelestialAngle(var1);
		return m_worldProvider->func_4096_a(var2, var1);
}

int World::func_4083_e(int var1, int var2) {
		Chunk* var3 = getChunkFromBlockCoords(var1, var2);

		size_t var4;
		for(var4 = 127; getBlockMaterial(var1, var4, var2)->func_880_c() && var4 > 0; --var4) {
		}

		var1 &= 15;

		for(var2 &= 15; var4 > 0; --var4) {
			int var5 = var3->getBlockID(var1, var4, var2);
			if(var5 != 0 && (Block::blocksList[var5]->m_blockMaterial->func_880_c() || Block::blocksList[var5]->m_blockMaterial->getIsLiquid())) {
				return var4 + 1;
			}
		}

		return -1;
}

int World::func_696_e(int var1, int var2) {
		return getChunkFromBlockCoords(var1, var2)->getHeightValue(var1 & 15, var2 & 15);
}

float World::func_679_f(float var1) {
		float var2 = getCelestialAngle(var1);
		float var3 = 1.0F - (cosf(var2 * 3.14159265358979323846f * 2.0F) * 2.0F + 12.0F / 16.0F);
		if(var3 < 0.0F) {
			var3 = 0.0F;
		}

		if(var3 > 1.0F) {
			var3 = 1.0F;
		}

		return var3 * var3 * 0.5F;
}

void World::scheduleBlockUpdate(int var1, int var2, int var3, int var4) {
		// HANDWRITTEN OVERRIDE: NextTickListEntry stored as a heap pointer in the vector "sets".
		NextTickListEntry* var5 = new NextTickListEntry(var1, var2, var3, var4);
		unsigned char var6 = 8;
		if(m_field_4214_a) {
			if(checkChunksExist(var5->xCoord - var6, var5->yCoord - var6, var5->zCoord - var6, var5->xCoord + var6, var5->yCoord + var6, var5->zCoord + var6)) {
				int var7 = getBlockId(var5->xCoord, var5->yCoord, var5->zCoord);
				if(var7 == var5->blockID && var7 > 0) {
					Block::blocksList[var7]->updateTick(this, var5->xCoord, var5->yCoord, var5->zCoord, rand);
				}
			}
			delete var5;

		} else {
			if(checkChunksExist(var1 - var6, var2 - var6, var3 - var6, var1 + var6, var2 + var6, var3 + var6)) {
				if(var4 > 0) {
					var5->setScheduledTime((long)Block::blocksList[var4]->tickRate() + m_worldTime);
				}

				// Java: if (!scheduledTickSet.contains(var5)) { add to both sets }
				bool _contains = false;
				for(size_t _i = 0; _i < m_scheduledTickSet->size(); ++_i) {
					if((*m_scheduledTickSet)[_i]->equals(var5)) { _contains = true; break; }
				}
				if(!_contains) {
					m_scheduledTickSet->push_back(var5);
					m_scheduledTickTreeSet->push_back(var5);
				} else {
					delete var5;
				}
			} else {
				delete var5;
			}

		}
}

void World::func_633_c() {
		_vecRemoveAllVoid(m_loadedEntityList, m_field_1024_A);

		size_t var1;
		Entity* var2 = NULL;
		size_t var3;
		size_t var4;
		for(var1 = 0; var1 < m_field_1024_A.size(); ++var1) {
			var2 = (Entity*)m_field_1024_A[var1];
			var3 = var2->m_field_657_ba;
			var4 = var2->m_field_654_bc;
			if(var2->m_field_621_aZ && chunkExists(var3, var4)) {
				getChunkFromChunkCoords(var3, var4)->func_1015_b(var2);
			}
		}

		for(var1 = 0; var1 < m_field_1024_A.size(); ++var1) {
			releaseEntitySkin((Entity*)m_field_1024_A[var1]);
		}

  m_field_1024_A.clear();

		for(var1 = 0; var1 < m_loadedEntityList.size(); ++var1) {
			var2 = (Entity*)m_loadedEntityList[var1];
			if(var2->m_ridingEntity != NULL) {
				if(!var2->m_ridingEntity->isDead && var2->m_ridingEntity->m_riddenByEntity == var2) {
					continue;
				}

				var2->m_ridingEntity->m_riddenByEntity = NULL;
				var2->m_ridingEntity = NULL;
			}

			if(!var2->isDead) {
				func_667_e(var2);
			}

			if(var2->isDead) {
				var3 = var2->m_field_657_ba;
				var4 = var2->m_field_654_bc;
				if(var2->m_field_621_aZ && chunkExists(var3, var4)) {
					getChunkFromChunkCoords(var3, var4)->func_1015_b(var2);
				}

				_vecRemoveAt(m_loadedEntityList, var1--);
				releaseEntitySkin(var2);
			}
		}

		for(var1 = 0; var1 < m_loadedTileEntityList.size(); ++var1) {
			TileEntity* var5 = (TileEntity*)m_loadedTileEntityList[var1];
			var5->updateEntity();
		}

}

void World::func_667_e(Entity* var1) {
		func_4084_a(var1, true);
}

void World::func_4084_a(Entity* var1, bool var2) {
		int var3 = (int)floor(var1->posX);
		int var4 = (int)floor(var1->posZ);
		unsigned char var5 = 16;
		if(var2 || checkChunksExist(var3 - var5, 0, var4 - var5, var3 + var5, 128, var4 + var5)) {
			var1->lastTickPosX = var1->posX;
			var1->lastTickPosY = var1->posY;
			var1->lastTickPosZ = var1->posZ;
			var1->prevRotationYaw = var1->rotationYaw;
			var1->prevRotationPitch = var1->rotationPitch;
			if(var2 && var1->m_field_621_aZ) {
				if(var1->m_ridingEntity != NULL) {
					var1->func_350_p();
				} else {
					var1->onUpdate();
				}
			}

			if(((var1->posX) != (var1->posX)) || _dIsInfinite(var1->posX)) {
				var1->posX = var1->lastTickPosX;
			}

			if(((var1->posY) != (var1->posY)) || _dIsInfinite(var1->posY)) {
				var1->posY = var1->lastTickPosY;
			}

			if(((var1->posZ) != (var1->posZ)) || _dIsInfinite(var1->posZ)) {
				var1->posZ = var1->lastTickPosZ;
			}

			if(_dIsNaN((double)var1->rotationPitch) || _dIsInfinite((double)var1->rotationPitch)) {
				var1->rotationPitch = var1->prevRotationPitch;
			}

			if(_dIsNaN((double)var1->rotationYaw) || _dIsInfinite((double)var1->rotationYaw)) {
				var1->rotationYaw = var1->prevRotationYaw;
			}

			int var6 = (int)floor(var1->posX / 16.0f);
			int var7 = (int)floor(var1->posY / 16.0f);
			int var8 = (int)floor(var1->posZ / 16.0f);
			if(!var1->m_field_621_aZ || var1->m_field_657_ba != var6 || var1->m_field_656_bb != var7 || var1->m_field_654_bc != var8) {
				if(var1->m_field_621_aZ && chunkExists(var1->m_field_657_ba, var1->m_field_654_bc)) {
					getChunkFromChunkCoords(var1->m_field_657_ba, var1->m_field_654_bc)->func_1016_a(var1, var1->m_field_656_bb);
				}

				if(chunkExists(var6, var8)) {
					var1->m_field_621_aZ = true;
					getChunkFromChunkCoords(var6, var8)->addEntity(var1);
				} else {
					var1->m_field_621_aZ = false;
				}
			}

			if(var2 && var1->m_field_621_aZ && var1->m_riddenByEntity != NULL) {
				if(!var1->m_riddenByEntity->isDead && var1->m_riddenByEntity->m_ridingEntity == var1) {
					func_667_e(var1->m_riddenByEntity);
				} else {
					var1->m_riddenByEntity->m_ridingEntity = NULL;
					var1->m_riddenByEntity = NULL;
				}
			}

		}
}

bool World::checkIfAABBIsClear(AxisAlignedBB var1) {
		std::vector<void*> var2 = getEntitiesWithinAABBExcludingEntity(NULL, var1);

		for(size_t var3 = 0; var3 < var2.size(); ++var3) {
			Entity* var4 = (Entity*)var2[var3];
			if(!var4->isDead && var4->m_field_618_ad) {
				return false;
			}
		}

		return true;
}

bool World::getIsAnyLiquid(AxisAlignedBB var1) {
		int var2 = (int)floor(var1.minX);
		int var3 = (int)floor(var1.maxX + 1.0f);
		int var4 = (int)floor(var1.minY);
		int var5 = (int)floor(var1.maxY + 1.0f);
		int var6 = (int)floor(var1.minZ);
		int var7 = (int)floor(var1.maxZ + 1.0f);
		if(var1.minX < 0.0f) {
			--var2;
		}

		if(var1.minY < 0.0f) {
			--var4;
		}

		if(var1.minZ < 0.0f) {
			--var6;
		}

		for(int var8 = var2; var8 < var3; ++var8) {
			for(int var9 = var4; var9 < var5; ++var9) {
				for(int var10 = var6; var10 < var7; ++var10) {
					Block* var11 = Block::blocksList[getBlockId(var8, var9, var10)];
					if(var11 != NULL && var11->m_blockMaterial->getIsLiquid()) {
						return true;
					}
				}
			}
		}

		return false;
}

bool World::isBoundingBoxBurning(AxisAlignedBB var1) {
		int var2 = (int)floor(var1.minX);
		int var3 = (int)floor(var1.maxX + 1.0f);
		int var4 = (int)floor(var1.minY);
		int var5 = (int)floor(var1.maxY + 1.0f);
		int var6 = (int)floor(var1.minZ);
		int var7 = (int)floor(var1.maxZ + 1.0f);

		for(int var8 = var2; var8 < var3; ++var8) {
			for(int var9 = var4; var9 < var5; ++var9) {
				for(int var10 = var6; var10 < var7; ++var10) {
					int var11 = getBlockId(var8, var9, var10);
					if(var11 == Block::s_fire->blockID || var11 == Block::s_lavaStill->blockID || var11 == Block::s_lavaMoving->blockID) {
						return true;
					}
				}
			}
		}

		return false;
}

bool World::func_682_a(AxisAlignedBB var1, Material* var2, Entity* var3) {
		int var4 = (int)floor(var1.minX);
		int var5 = (int)floor(var1.maxX + 1.0f);
		int var6 = (int)floor(var1.minY);
		int var7 = (int)floor(var1.maxY + 1.0f);
		int var8 = (int)floor(var1.minZ);
		int var9 = (int)floor(var1.maxZ + 1.0f);
		bool var10 = false;
		Vec3D* var11 = Vec3D::createVector(0.0f, 0.0f, 0.0f);

		for(int var12 = var4; var12 < var5; ++var12) {
			for(int var13 = var6; var13 < var7; ++var13) {
				for(int var14 = var8; var14 < var9; ++var14) {
					Block* var15 = Block::blocksList[getBlockId(var12, var13, var14)];
					if(var15 != NULL && var15->m_blockMaterial == var2) {
						double var16 = (double)((float)(var13 + 1) - BlockFluids::func_288_b(getBlockMetadata(var12, var13, var14)));
						if((double)var7 >= var16) {
							var10 = true;
							var15->velocityToAddToEntity(this, var12, var13, var14, var3, var11);
						}
					}
				}
			}
		}

		if(var11->lengthVector() > 0.0f) {
			var11 = var11->normalize();
			double var18 = 0.004f;
			var3->motionX += var11->xCoord * var18;
			var3->motionY += var11->yCoord * var18;
			var3->motionZ += var11->zCoord * var18;
		}

		return var10;
}

bool World::func_689_a(AxisAlignedBB var1, Material* var2) {
		int var3 = (int)floor(var1.minX);
		int var4 = (int)floor(var1.maxX + 1.0f);
		int var5 = (int)floor(var1.minY);
		int var6 = (int)floor(var1.maxY + 1.0f);
		int var7 = (int)floor(var1.minZ);
		int var8 = (int)floor(var1.maxZ + 1.0f);

		for(int var9 = var3; var9 < var4; ++var9) {
			for(int var10 = var5; var10 < var6; ++var10) {
				for(int var11 = var7; var11 < var8; ++var11) {
					Block* var12 = Block::blocksList[getBlockId(var9, var10, var11)];
					if(var12 != NULL && var12->m_blockMaterial == var2) {
						return true;
					}
				}
			}
		}

		return false;
}

bool World::func_707_b(AxisAlignedBB var1, Material* var2) {
		int var3 = (int)floor(var1.minX);
		int var4 = (int)floor(var1.maxX + 1.0f);
		int var5 = (int)floor(var1.minY);
		int var6 = (int)floor(var1.maxY + 1.0f);
		int var7 = (int)floor(var1.minZ);
		int var8 = (int)floor(var1.maxZ + 1.0f);

		for(int var9 = var3; var9 < var4; ++var9) {
			for(int var10 = var5; var10 < var6; ++var10) {
				for(int var11 = var7; var11 < var8; ++var11) {
					Block* var12 = Block::blocksList[getBlockId(var9, var10, var11)];
					if(var12 != NULL && var12->m_blockMaterial == var2) {
						int var13 = getBlockMetadata(var9, var10, var11);
						double var14 = (double)(var10 + 1);
						if(var13 < 8) {
							var14 = (double)(var10 + 1) - (double)var13 / 8.0f;
						}

						if(var14 >= var1.minY) {
							return true;
						}
					}
				}
			}
		}

		return false;
}

Explosion* World::func_12243_a(Entity* var1, double var2, double var4, double var6, float var8) {
		return func_12244_a(var1, var2, var4, var6, var8, false);
}

Explosion* World::func_12244_a(Entity* var1, double var2, double var4, double var6, float var8, bool var9) {
		Explosion* var10 = new Explosion(this, var1, var2, var4, var6, var8);
		var10->m_field_12257_a = var9;
		var10->func_12248_a();
		var10->func_12247_b();
		return var10;
}

float World::func_675_a(Vec3D* var1, AxisAlignedBB var2) {
		double var3 = 1.0f / ((var2.maxX - var2.minX) * 2.0f + 1.0f);
		double var5 = 1.0f / ((var2.maxY - var2.minY) * 2.0f + 1.0f);
		double var7 = 1.0f / ((var2.maxZ - var2.minZ) * 2.0f + 1.0f);
		int var9 = 0;
		int var10 = 0;

		for(float var11 = 0.0F; var11 <= 1.0F; var11 = (float)((double)var11 + var3)) {
			for(float var12 = 0.0F; var12 <= 1.0F; var12 = (float)((double)var12 + var5)) {
				for(float var13 = 0.0F; var13 <= 1.0F; var13 = (float)((double)var13 + var7)) {
					double var14 = var2.minX + (var2.maxX - var2.minX) * (double)var11;
					double var16 = var2.minY + (var2.maxY - var2.minY) * (double)var12;
					double var18 = var2.minZ + (var2.maxZ - var2.minZ) * (double)var13;
					if(rayTraceBlocks(Vec3D::createVector(var14, var16, var18), var1) == NULL) {
						++var9;
					}

					++var10;
				}
			}
		}

		return (float)var9 / (float)var10;
}

void World::onBlockHit(int var1, int var2, int var3, int var4) {
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
			playSoundEffect((double)((float)var1 + 0.5F), (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), "m_random->fizz", 0.5F, 2.6F + (rand->nextFloat() - rand->nextFloat()) * 0.8F);
			setBlockWithNotify(var1, var2, var3, 0);
		}

}

Entity* World::func_4085_a(void* var1) {
		return NULL;
}

std::string World::func_687_d() {
		return std::string("All: ") + toStr((int)m_loadedEntityList.size());
}

TileEntity* World::getBlockTileEntity(int var1, int var2, int var3) {
		Chunk* var4 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
		return var4 != NULL ? var4->getChunkBlockTileEntity(var1 & 15, var2, var3 & 15) : NULL;
}

void World::setBlockTileEntity(int var1, int var2, int var3, TileEntity* var4) {
		Chunk* var5 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
		if(var5 != NULL) {
			var5->setChunkBlockTileEntity(var1 & 15, var2, var3 & 15, var4);
		}

}

void World::removeBlockTileEntity(int var1, int var2, int var3) {
		Chunk* var4 = getChunkFromChunkCoords(var1 >> 4, var3 >> 4);
		if(var4 != NULL) {
			var4->removeChunkBlockTileEntity(var1 & 15, var2, var3 & 15);
		}

}

bool World::isBlockOpaqueCube(int var1, int var2, int var3) {
		Block* var4 = Block::blocksList[getBlockId(var1, var2, var3)];
		return var4 == NULL ? false : var4->isOpaqueCube();
}

void World::func_651_a(IProgressUpdate* var1) {
		saveWorld(true, var1);
}

bool World::func_6465_g() {
		if(m_field_4204_J >= 50) {
			return false;
		} else {
			++m_field_4204_J;

   { // was try
				int var1 = 5000;

				bool var2;
				while(m_field_1051_z.size() > 0) {
					--var1;
					if(var1 <= 0) {
						var2 = true;
						return var2;
					}

					{
						// HANDWRITTEN: m_field_1051_z holds MetadataChunkBlock* (heap); pop last, run, free.
						MetadataChunkBlock* _mcb = (MetadataChunkBlock*)_vecRemoveAt(m_field_1051_z, (int)m_field_1051_z.size() - 1);
						_mcb->func_4127_a(this);
						delete _mcb;
					}
				}

				var2 = false;
				return var2;
   } { // was finally — cleanup code follows
				--m_field_4204_J;
			}
		}
}

void World::func_616_a(EnumSkyBlock* var1, int var2, int var3, int var4, int var5, int var6, int var7) {
		func_627_a(var1, var2, var3, var4, var5, var6, var7, true);
}

void World::func_627_a(EnumSkyBlock* var1, int var2, int var3, int var4, int var5, int var6, int var7, bool var8) {
		if(!m_worldProvider->m_field_6478_e || var1 != EnumSkyBlock::s_Sky) {
			++s_field_9429_y;
			if(s_field_9429_y == 50) {
				--s_field_9429_y;
			} else {
				int var9 = (var5 + var2) / 2;
				int var10 = (var7 + var4) / 2;
				if(!blockExists(var9, 64, var10)) {
					--s_field_9429_y;
				} else {
					int var11 = m_field_1051_z.size();
					if(var8) {
						int var12 = 4;
						if(var12 > var11) {
							var12 = var11;
						}

						for(int var13 = 0; var13 < var12; ++var13) {
							MetadataChunkBlock* var14 = (MetadataChunkBlock*)m_field_1051_z[m_field_1051_z.size() - var13 - 1];
							if(var14->m_field_1299_a == var1 && var14->func_866_a(var2, var3, var4, var5, var6, var7)) {
								--s_field_9429_y;
								return;
							}
						}
					}

					m_field_1051_z.push_back(new MetadataChunkBlock(var1, var2, var3, var4, var5, var6, var7));
					if(m_field_1051_z.size() > 100000) {
      m_field_1051_z.clear();
					}

					--s_field_9429_y;
				}
			}
		}
}

void World::calculateInitialSkylight() {
		int var1 = calculateSkylightSubtracted(1.0F);
		if(var1 != m_skylightSubtracted) {
			m_skylightSubtracted = var1;
		}

}

void World::tick() {
		SpawnerAnimals::performSpawning(this);
		m_chunkProvider->func_532_a();
		int var1 = calculateSkylightSubtracted(1.0F);
		if(var1 != m_skylightSubtracted) {
			m_skylightSubtracted = var1;

			for(size_t var2 = 0; var2 < m_worldAccesses.size(); ++var2) {
				m_worldAccesses[var2]->func_936_e();
			}
		}

		++m_worldTime;
		if(m_worldTime % (long)m_autosavePeriod == 0L) {
			saveWorld(false, NULL);
		}

		TickUpdates(false);
		func_4080_j();
}

void World::func_4080_j() {
		// HANDWRITTEN: free last tick's ChunkCoordIntPair entries before refilling (avoid per-tick leak).
		for(size_t _d = 0; _d < m_field_9427_K->size(); ++_d) {
			delete (ChunkCoordIntPair*)(*m_field_9427_K)[_d];
		}
		m_field_9427_K->clear();

		size_t var3;
		size_t var4;
		int var6;
		size_t var7;
		for(size_t var1 = 0; var1 < m_playerEntities.size(); ++var1) {
			EntityPlayer* var2 = (EntityPlayer*)m_playerEntities[var1];
			var3 = (int)floor(var2->posX / 16.0f);
			var4 = (int)floor(var2->posZ / 16.0f);
			unsigned char var5 = 9;

			for(var6 = -var5; var6 <= var5; ++var6) {
				for(var7 = -var5; var7 <= var5; ++var7) {
					m_field_9427_K->push_back(new ChunkCoordIntPair(var6 + var3, var7 + var4));
				}
			}
		}

		if(m_field_9426_L > 0) {
			--m_field_9426_L;
		}

		for (size_t _i12 = 0; _i12 < m_field_9427_K->size(); ++_i12) {
			ChunkCoordIntPair* var13 = (ChunkCoordIntPair*)(*m_field_9427_K)[_i12];
			var3 = var13->m_chunkXPos * 16;
			var4 = var13->m_chunkZPos * 16;
			Chunk* var14 = getChunkFromChunkCoords(var13->m_chunkXPos, var13->m_chunkZPos);
			int var8;
			int var9;
			size_t var10;
			if(m_field_9426_L == 0) {
				m_field_9437_g = m_field_9437_g * 3 + m_field_9436_h;
				var6 = m_field_9437_g >> 2;
				var7 = var6 & 15;
				var8 = var6 >> 8 & 15;
				var9 = var6 >> 16 & 127;
				var10 = var14->getBlockID(var7, var9, var8);
				var7 += var3;
				var8 += var4;
				if(var10 == 0 && getBlockLightValue(var7, var9, var8) <= rand->nextInt(8) && getSavedLightValue(EnumSkyBlock::s_Sky, var7, var9, var8) <= 0) {
					EntityPlayer* var11 = getClosestPlayer((double)var7 + 0.5f, (double)var9 + 0.5f, (double)var8 + 0.5f, 8.0f);
					if(var11 != NULL && var11->getDistanceSq((double)var7 + 0.5f, (double)var9 + 0.5f, (double)var8 + 0.5f) > 4.0f) {
						playSoundEffect((double)var7 + 0.5f, (double)var9 + 0.5f, (double)var8 + 0.5f, "ambient.cave.cave", 0.7F, 0.8F + rand->nextFloat() * 0.2F);
						m_field_9426_L = rand->nextInt(12000) + 6000;
					}
				}
			}

			for(var6 = 0; var6 < 80; ++var6) {
				m_field_9437_g = m_field_9437_g * 3 + m_field_9436_h;
				var7 = m_field_9437_g >> 2;
				var8 = var7 & 15;
				var9 = var7 >> 8 & 15;
				var10 = var7 >> 16 & 127;
				unsigned char var15 = var14->m_blocks[var8 << 11 | var9 << 7 | var10];
				if(Block::s_tickOnLoad[var15]) {
					Block::blocksList[var15]->updateTick(this, var8 + var3, var10, var9 + var4, rand);
				}
			}
		}

}

bool World::TickUpdates(bool var1) {
		// HANDWRITTEN OVERRIDE: TreeSet emulated by scanning the vector for the minimum entry
		// (NextTickListEntry::a / compareTo gives scheduledTime-then-id order = TreeSet.first()).
		int var2 = (int)m_scheduledTickTreeSet->size();
		if(var2 != (int)m_scheduledTickSet->size()) {
			DbgPrintF("ERROR [IllegalStateException]: %s\n", "TickNextTick list out of synch");
			return false;
		} else {
			if(var2 > 1000) {
				var2 = 1000;
			}

			for(int var3 = 0; var3 < var2; ++var3) {
				if(m_scheduledTickTreeSet->empty()) {
					break;
				}

				// TreeSet.first(): smallest by compareTo.
				size_t _minIdx = 0;
				for(size_t _i = 1; _i < m_scheduledTickTreeSet->size(); ++_i) {
					if((*m_scheduledTickTreeSet)[_i]->a(*(*m_scheduledTickTreeSet)[_minIdx]) < 0) {
						_minIdx = _i;
					}
				}
				NextTickListEntry* var4 = (*m_scheduledTickTreeSet)[_minIdx];
				if(!var1 && var4->m_scheduledTime > m_worldTime) {
					break;
				}

				// remove var4 from both vectors ("sets")
				m_scheduledTickTreeSet->erase(m_scheduledTickTreeSet->begin() + _minIdx);
				for(size_t _j = 0; _j < m_scheduledTickSet->size(); ++_j) {
					if((*m_scheduledTickSet)[_j] == var4) {
						m_scheduledTickSet->erase(m_scheduledTickSet->begin() + _j);
						break;
					}
				}

				unsigned char var5 = 8;
				if(checkChunksExist(var4->xCoord - var5, var4->yCoord - var5, var4->zCoord - var5, var4->xCoord + var5, var4->yCoord + var5, var4->zCoord + var5)) {
					int var6 = getBlockId(var4->xCoord, var4->yCoord, var4->zCoord);
					if(var6 == var4->blockID && var6 > 0) {
						Block::blocksList[var6]->updateTick(this, var4->xCoord, var4->yCoord, var4->zCoord, rand);
					}
				}
				delete var4;
			}

			return m_scheduledTickTreeSet->size() != 0;
		}
}

void World::randomDisplayUpdates(int var1, int var2, int var3) {
		unsigned char var4 = 16;
		Random* var5 = new Random();

		for(int var6 = 0; var6 < 1000; ++var6) {
			int var7 = var1 + rand->nextInt(var4) - rand->nextInt(var4);
			int var8 = var2 + rand->nextInt(var4) - rand->nextInt(var4);
			int var9 = var3 + rand->nextInt(var4) - rand->nextInt(var4);
			int var10 = getBlockId(var7, var8, var9);
			if(var10 > 0) {
				Block::blocksList[var10]->randomDisplayTick(this, var7, var8, var9, var5);
			}
		}

}

std::vector<void*> /* PORT: determine element type */ World::getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB var2) {
  m_field_1012_M.clear();
		int var3 = (int)floor((var2.minX - 2.0f) / 16.0f);
		int var4 = (int)floor((var2.maxX + 2.0f) / 16.0f);
		int var5 = (int)floor((var2.minZ - 2.0f) / 16.0f);
		int var6 = (int)floor((var2.maxZ + 2.0f) / 16.0f);

		for(int var7 = var3; var7 <= var4; ++var7) {
			for(int var8 = var5; var8 <= var6; ++var8) {
				if(chunkExists(var7, var8)) {
					getChunkFromChunkCoords(var7, var8)->getEntitiesWithinAABBForEntity(var1, var2, m_field_1012_M);
				}
			}
		}

		return m_field_1012_M;
}

std::vector<void*> /* PORT: determine element type */ World::getEntitiesWithinAABB(void* var1, AxisAlignedBB var2) {
		int var3 = (int)floor((var2.minX - 2.0f) / 16.0f);
		int var4 = (int)floor((var2.maxX + 2.0f) / 16.0f);
		int var5 = (int)floor((var2.minZ - 2.0f) / 16.0f);
		int var6 = (int)floor((var2.maxZ + 2.0f) / 16.0f);
		std::vector<void*> var7 /* PORT: determine element type */;

		for(int var8 = var3; var8 <= var4; ++var8) {
			for(int var9 = var5; var9 <= var6; ++var9) {
				if(chunkExists(var8, var9)) {
					getChunkFromChunkCoords(var8, var9)->getEntitiesOfTypeWithinAAAB(var1, var2, var7);
				}
			}
		}

		return var7;
}

std::vector<void*> /* PORT: determine element type */ World::func_658_i() {
		return m_loadedEntityList;
}

void World::func_698_b(int var1, int var2, int var3, TileEntity* var4) {
		if(blockExists(var1, var2, var3)) {
			getChunkFromBlockCoords(var1, var3)->setChunkModified();
		}

		for(size_t var5 = 0; var5 < m_worldAccesses.size(); ++var5) {
			m_worldAccesses[var5]->func_935_a(var1, var2, var3, var4);
		}

}

int World::countEntities(void* var1) {
		int var2 = 0;

		for(size_t var3 = 0; var3 < m_loadedEntityList.size(); ++var3) {
			Entity* var4 = (Entity*)m_loadedEntityList[var3];
			if(true) {
				++var2;
			}
		}

		return var2;
}

void World::func_636_a(std::vector<void*> /* PORT: determine element type */ var1) {
		m_loadedEntityList.insert(m_loadedEntityList.end(), var1.begin(), var1.end());

		for(size_t var2 = 0; var2 < var1.size(); ++var2) {
			obtainEntitySkin((Entity*)var1[var2]);
		}

}

void World::func_632_b(std::vector<void*> /* PORT: determine element type */ var1) {
		m_field_1024_A.insert(m_field_1024_A.end(), var1.begin(), var1.end());
}

void World::func_656_j() {
		while(m_chunkProvider->func_532_a()) {
		}

}

bool World::canBlockBePlacedAt(int var1, int var2, int var3, int var4, bool var5) {
		int var6 = getBlockId(var2, var3, var4);
		Block* var7 = Block::blocksList[var6];
		Block* var8 = Block::blocksList[var1];
		AxisAlignedBB var9 = var8->getCollisionBoundingBoxFromPool(this, var2, var3, var4);
		// HANDWRITTEN: Java set var9=null when var5 to skip the clearance check (AxisAlignedBB is a
		// value struct here, so use a flag instead of a null AABB).
		bool _aabbClear = var5 ? true : checkIfAABBIsClear(var9);

		return true && !_aabbClear ? false : (var7 != Block::s_waterStill && var7 != Block::s_waterMoving && var7 != Block::s_lavaStill && var7 != Block::s_lavaMoving && var7 != Block::s_fire && var7 != Block::s_snow ? var1 > 0 && var7 == NULL && var8->canPlaceBlockAt(this, var2, var3, var4) : true);
}

PathEntity* World::getPathToEntity(Entity* var1, Entity* var2, float var3) {
		int var4 = (int)floor(var1->posX);
		int var5 = (int)floor(var1->posY);
		int var6 = (int)floor(var1->posZ);
		int var7 = (int)(var3 + 16.0F);
		int var8 = var4 - var7;
		int var9 = var5 - var7;
		int var10 = var6 - var7;
		int var11 = var4 + var7;
		int var12 = var5 + var7;
		int var13 = var6 + var7;
		ChunkCache* var14 = new ChunkCache(this, var8, var9, var10, var11, var12, var13);
		return (new Pathfinder(var14))->createEntityPathTo(var1, var2, var3);
}

PathEntity* World::getEntityPathToXYZ(Entity* var1, int var2, int var3, int var4, float var5) {
		int var6 = (int)floor(var1->posX);
		int var7 = (int)floor(var1->posY);
		int var8 = (int)floor(var1->posZ);
		int var9 = (int)(var5 + 8.0F);
		int var10 = var6 - var9;
		int var11 = var7 - var9;
		int var12 = var8 - var9;
		int var13 = var6 + var9;
		int var14 = var7 + var9;
		int var15 = var8 + var9;
		ChunkCache* var16 = new ChunkCache(this, var10, var11, var12, var13, var14, var15);
		return (new Pathfinder(var16))->createEntityPathTo(var1, var2, var3, var4, var5);
}

bool World::isBlockProvidingPowerTo(int var1, int var2, int var3, int var4) {
		int var5 = getBlockId(var1, var2, var3);
		return var5 == 0 ? false : Block::blocksList[var5]->isIndirectlyPoweringTo(this, var1, var2, var3, var4);
}

bool World::isBlockGettingPowered(int var1, int var2, int var3) {
		return isBlockProvidingPowerTo(var1, var2 - 1, var3, 0) ? true : (isBlockProvidingPowerTo(var1, var2 + 1, var3, 1) ? true : (isBlockProvidingPowerTo(var1, var2, var3 - 1, 2) ? true : (isBlockProvidingPowerTo(var1, var2, var3 + 1, 3) ? true : (isBlockProvidingPowerTo(var1 - 1, var2, var3, 4) ? true : isBlockProvidingPowerTo(var1 + 1, var2, var3, 5)))));
}

bool World::isBlockIndirectlyProvidingPowerTo(int var1, int var2, int var3, int var4) {
		if(isBlockOpaqueCube(var1, var2, var3)) {
			return isBlockGettingPowered(var1, var2, var3);
		} else {
			int var5 = getBlockId(var1, var2, var3);
			return var5 == 0 ? false : Block::blocksList[var5]->isPoweringTo(this, var1, var2, var3, var4);
		}
}

bool World::isBlockIndirectlyGettingPowered(int var1, int var2, int var3) {
		return isBlockIndirectlyProvidingPowerTo(var1, var2 - 1, var3, 0) ? true : (isBlockIndirectlyProvidingPowerTo(var1, var2 + 1, var3, 1) ? true : (isBlockIndirectlyProvidingPowerTo(var1, var2, var3 - 1, 2) ? true : (isBlockIndirectlyProvidingPowerTo(var1, var2, var3 + 1, 3) ? true : (isBlockIndirectlyProvidingPowerTo(var1 - 1, var2, var3, 4) ? true : isBlockIndirectlyProvidingPowerTo(var1 + 1, var2, var3, 5)))));
}

EntityPlayer* World::getClosestPlayerToEntity(Entity* var1, double var2) {
		return getClosestPlayer(var1->posX, var1->posY, var1->posZ, var2);
}

EntityPlayer* World::getClosestPlayer(double var1, double var3, double var5, double var7) {
		double var9 = -1.0f;
		EntityPlayer* var11 = NULL;

		for(size_t var12 = 0; var12 < m_playerEntities.size(); ++var12) {
			EntityPlayer* var13 = (EntityPlayer*)m_playerEntities[var12];
			double var14 = var13->getDistanceSq(var1, var3, var5);
			if((var7 < 0.0f || var14 < var7 * var7) && (var9 == -1.0f || var14 < var9)) {
				var9 = var14;
				var11 = var13;
			}
		}

		return var11;
}

void World::func_693_a(int var1, int var2, int var3, int var4, int var5, int var6, unsigned char* var7) {
		int var8 = var1 >> 4;
		int var9 = var3 >> 4;
		int var10 = (var1 + var4 - 1) >> 4;
		int var11 = (var3 + var6 - 1) >> 4;
		int var12 = 0;
		int var13 = var2;
		int var14 = var2 + var5;
		if(var2 < 0) {
			var13 = 0;
		}

		if(var14 > 128) {
			var14 = 128;
		}

		for(int var15 = var8; var15 <= var10; ++var15) {
			int var16 = var1 - var15 * 16;
			int var17 = var1 + var4 - var15 * 16;
			if(var16 < 0) {
				var16 = 0;
			}

			if(var17 > 16) {
				var17 = 16;
			}

			for(int var18 = var9; var18 <= var11; ++var18) {
				int var19 = var3 - var18 * 16;
				int var20 = var3 + var6 - var18 * 16;
				if(var19 < 0) {
					var19 = 0;
				}

				if(var20 > 16) {
					var20 = 16;
				}

				var12 = getChunkFromChunkCoords(var15, var18)->func_1004_a(var7, var16, var13, var19, var17, var14, var20, var12);
				func_701_b(var15 * 16 + var16, var13, var18 * 16 + var19, var15 * 16 + var17, var14, var18 * 16 + var20);
			}
		}

}

void World::sendQuittingDisconnectingPacket() {
	// TODO: implement
}

void World::func_663_l() {
		// PORT: the session.lock multi-access guard relied on java.io.File/DataInputStream.
		// On 360 a world container is opened exclusively via the STFS save path (ContentStorage),
		// which is inherently single-accessor, so this check is a no-op here.
}

void World::setWorldTime(long long var1) {
		m_worldTime = var1;
}

void World::func_705_f(Entity* var1) {
		int var2 = (int)floor(var1->posX / 16.0f);
		int var3 = (int)floor(var1->posZ / 16.0f);
		unsigned char var4 = 2;

		for(int var5 = var2 - var4; var5 <= var2 + var4; ++var5) {
			for(int var6 = var3 - var4; var6 <= var3 + var4; ++var6) {
				getChunkFromChunkCoords(var5, var6);
			}
		}

		// Java: if (!loadedEntityList.contains(var1)) loadedEntityList.add(var1);
		if(std::find(m_loadedEntityList.begin(), m_loadedEntityList.end(), (void*)var1) == m_loadedEntityList.end()) {
			m_loadedEntityList.push_back(var1);
		}

}

bool World::func_6466_a(EntityPlayer* var1, int var2, int var3, int var4) {
		return true;
}

void World::func_9425_a(Entity* var1, unsigned char var2) {
	// TODO: implement
}

void World::func_9424_o() {
		_vecRemoveAllVoid(m_loadedEntityList, m_field_1024_A);

		size_t var1;
		Entity* var2 = NULL;
		size_t var3;
		size_t var4;
		for(var1 = 0; var1 < m_field_1024_A.size(); ++var1) {
			var2 = (Entity*)m_field_1024_A[var1];
			var3 = var2->m_field_657_ba;
			var4 = var2->m_field_654_bc;
			if(var2->m_field_621_aZ && chunkExists(var3, var4)) {
				getChunkFromChunkCoords(var3, var4)->func_1015_b(var2);
			}
		}

		for(var1 = 0; var1 < m_field_1024_A.size(); ++var1) {
			releaseEntitySkin((Entity*)m_field_1024_A[var1]);
		}

  m_field_1024_A.clear();

		for(var1 = 0; var1 < m_loadedEntityList.size(); ++var1) {
			var2 = (Entity*)m_loadedEntityList[var1];
			if(var2->m_ridingEntity != NULL) {
				if(!var2->m_ridingEntity->isDead && var2->m_ridingEntity->m_riddenByEntity == var2) {
					continue;
				}

				var2->m_ridingEntity->m_riddenByEntity = NULL;
				var2->m_ridingEntity = NULL;
			}

			if(var2->isDead) {
				var3 = var2->m_field_657_ba;
				var4 = var2->m_field_654_bc;
				if(var2->m_field_621_aZ && chunkExists(var3, var4)) {
					getChunkFromChunkCoords(var3, var4)->func_1015_b(var2);
				}

				_vecRemoveAt(m_loadedEntityList, var1--);
				releaseEntitySkin(var2);
			}
		}

}
