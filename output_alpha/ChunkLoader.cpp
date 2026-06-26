#include "stdafx.h"
// Hand-ported from ChunkLoader.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Per-chunk NBT save/load to disk. LOGIC (file I/O + NBT).
//
// PORT NOTES:
//  - java.io.File / FileInputStream / FileOutputStream come from the JavaIO.h
//    bridge (File.h -> JavaIO.h), backed by Win32/Xbox CreateFile on a mounted
//    STFS drive. The bridge's File has exists()/mkdirs()/deleteFile() but NO
//    length() or renameTo(); the save path is adapted accordingly (see below).
//  - The Java original gzipped the NBT through CompressedStreamTools stream APIs.
//    Here CompressedStreamTools::func_1138_a(InputStream*) is a NULL stub, so the
//    raw (uncompressed) byte-buffer variants func_1140_a / func_1142_a are used
//    instead, matching the rest of the port (saves are stored raw NBT).
//  - m_saveDir is the save root path (std::string*), per the converted header.
//  - The chunk path keeps Alpha's base36 dir layout: <saveDir>/<x&63>/<z&63>/c.<x>.<z>.dat

#include "ChunkLoader.h"
#include "Common.h"
#include "Chunk.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTBase.h"
#include "World.h"
#include "Entity.h"
#include "TileEntity.h"
#include "CompressedStreamTools.h"
#include "EntityList.h"
#include "NibbleArray.h"
#include "JavaIO.h"
#include <string>
#include <vector>

// PORT: Integer.toString(value, 36) — Java base-36 string (handles negatives).
static std::string intToBase36(int value) {
	if(value == 0) {
		return std::string("0");
	}

	bool negative = value < 0;
	// Work in long long to avoid INT_MIN overflow when negating.
	long long v = (long long)value;
	if(negative) {
		v = -v;
	}

	const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	std::string out;
	while(v > 0) {
		out += digits[(int)(v % 36)];
		v /= 36;
	}

	if(negative) {
		out += '-';
	}

	// Reverse in place.
	for(int i = 0, j = (int)out.size() - 1; i < j; ++i, --j) {
		char t = out[i];
		out[i] = out[j];
		out[j] = t;
	}

	return out;
}

ChunkLoader::ChunkLoader()
	: m_saveDir(NULL)
	, m_createIfNecessary(false)
{
}

ChunkLoader::ChunkLoader(std::string /* PORT: was java.io.File */* var1, bool var2)
	: m_saveDir(NULL)
	, m_createIfNecessary(false)
{
	m_saveDir = var1;
	m_createIfNecessary = var2;
}

ChunkLoader::~ChunkLoader() {
	m_saveDir = NULL;
}

// Builds (and, if createIfNecessary, creates the dirs for) the on-disk path of a
// chunk. Returns a heap std::string* with the full path, or NULL when the file
// does not exist and we are not allowed to create it. Caller owns the pointer.
std::string /* PORT: was java.io.File */* ChunkLoader::chunkFileForXZ(int var1, int var2) {
	if(m_saveDir == NULL) {
		return NULL;
	}

	std::string var3 = "c." + intToBase36(var1) + "." + intToBase36(var2) + ".dat";
	std::string var4 = intToBase36(var1 & 63);
	std::string var5 = intToBase36(var2 & 63);

	File var6(*m_saveDir, var4);
	if(!var6.exists()) {
		if(!m_createIfNecessary) {
			return NULL;
		}

		var6.mkdirs();
	}

	File var7(&var6, var5);
	if(!var7.exists()) {
		if(!m_createIfNecessary) {
			return NULL;
		}

		var7.mkdirs();
	}

	File var8(&var7, var3);
	if(!var8.exists() && !m_createIfNecessary) {
		return NULL;
	}

	return new std::string(var8.getPath());
}

Chunk* ChunkLoader::loadChunk(World* var1, int var2, int var3) {
	std::string* var4 = chunkFileForXZ(var2, var3);
	if(var4 != NULL) {
		File var5(*var4);
		if(var5.exists()) {
			// PORT: read the raw NBT bytes from the file, then parse via the
			// byte-buffer reader (the InputStream overload is a NULL stub).
			FileInputStream var6(&var5);
			std::vector<unsigned char> var7;
			unsigned char var8[4096];
			int var9;
			while((var9 = var6.read(var8, (int)sizeof(var8))) > 0) {
				var7.insert(var7.end(), var8, var8 + var9);
			}
			var6.close();

			NBTTagCompound* var10 = var7.empty() ? NULL : CompressedStreamTools::func_1140_a(&var7[0]);
			if(var10 == NULL) {
				delete var4;
				return NULL;
			}

			if(!var10->hasKey("Level")) {
				DbgPrintF("%s\n", ("Chunk file at " + toStr(var2) + "," + toStr(var3) + " is missing level data, skipping").c_str());
				delete var4;
				return NULL;
			}

			if(!var10->getCompoundTag("Level")->hasKey("Blocks")) {
				DbgPrintF("%s\n", ("Chunk file at " + toStr(var2) + "," + toStr(var3) + " is missing block data, skipping").c_str());
				delete var4;
				return NULL;
			}

			Chunk* var11 = loadChunkIntoWorldFromCompound(var1, var10->getCompoundTag("Level"));
			if(!var11->isAtLocation(var2, var3)) {
				DbgPrintF("%s\n", ("Chunk file at " + toStr(var2) + "," + toStr(var3) + " is in the wrong location; relocating. (Expected " + toStr(var2) + ", " + toStr(var3) + ", got " + toStr(var11->m_xPosition) + ", " + toStr(var11->m_zPosition) + ")").c_str());
				var10->setInteger("xPos", var2);
				var10->setInteger("zPos", var3);
				var11 = loadChunkIntoWorldFromCompound(var1, var10->getCompoundTag("Level"));
			}

			delete var4;
			return var11;
		}

		delete var4;
	}

	return NULL;
}

void ChunkLoader::saveChunk(World* var1, Chunk* var2) {
	var1->func_663_l();
	std::string* var3 = chunkFileForXZ(var2->m_xPosition, var2->m_zPosition);
	if(var3 == NULL) {
		// PORT: createIfNecessary was false and the dirs don't exist -> nothing to do.
		return;
	}

	// PORT: the JavaIO File bridge has no length()/renameTo(); Java's
	// sizeOnDisk bookkeeping (subtract old length, write tmp_chunk.dat, delete,
	// rename, add new length) is simplified to a direct overwrite of the final
	// file. sizeOnDisk is left untouched here (informational only).
	NBTTagCompound* var4 = new NBTTagCompound();
	NBTTagCompound* var5 = new NBTTagCompound();
	var4->setTag("Level", var5);
	storeChunkInCompound(var2, var1, var5);

	// Serialize the compound to a raw byte buffer and write it straight to the
	// chunk file (uncompressed NBT, matching the rest of the port).
	DataOutput var6;
	CompressedStreamTools::func_1139_a(var4, &var6);

	FileOutputStream var7(*var3);
	if(!var6.buffer.empty()) {
		var7.write(&var6.buffer[0], (int)var6.buffer.size());
	}
	var7.close();

	delete var4; // owns Level subtag + entity/tileentity tags built below
	delete var3;
}

void ChunkLoader::storeChunkInCompound(Chunk* var1, World* var2, NBTTagCompound* var3) {
	var2->func_663_l();
	var3->setInteger("xPos", var1->m_xPosition);
	var3->setInteger("zPos", var1->m_zPosition);
	var3->setLong("LastUpdate", var2->m_worldTime);
	var3->setByteArray("Blocks", var1->m_blocks);
	var3->setByteArray("Data", var1->m_data->m_data);
	var3->setByteArray("SkyLight", var1->m_skylightMap->m_data);
	var3->setByteArray("BlockLight", var1->m_blocklightMap->m_data);
	var3->setByteArray("HeightMap", var1->m_heightMap);
	var3->setBoolean("TerrainPopulated", var1->m_isTerrainPopulated);
	var1->m_hasEntities = false;
	NBTTagList* var4 = new NBTTagList();

	// Chunk::m_entities is an array of std::vector<void*> buckets (by Y),
	// m_entities_count buckets total; each element is an Entity*.
	NBTTagCompound* var8 = NULL;
	for(int var5 = 0; var5 < var1->m_entities_count; ++var5) {
		std::vector<void*>& var6 = var1->m_entities[var5];
		for(size_t var7 = 0; var7 < var6.size(); ++var7) {
			Entity* var9 = (Entity*)var6[var7];
			var1->m_hasEntities = true;
			var8 = new NBTTagCompound();
			if(var9->func_358_c(var8)) {
				var4->setTag(var8);
			}
		}
	}

	var3->setTag("Entities", var4);
	NBTTagList* var10 = new NBTTagList();

	// Chunk::m_chunkTileEntityMap is a std::map<std::string, void*>* of TileEntity*.
	if(var1->m_chunkTileEntityMap != NULL) {
		for(std::map<std::string, void*>::iterator var11 = var1->m_chunkTileEntityMap->begin(); var11 != var1->m_chunkTileEntityMap->end(); ++var11) {
			TileEntity* var12 = (TileEntity*)var11->second;
			var8 = new NBTTagCompound();
			var12->writeToNBT(var8);
			var10->setTag(var8);
		}
	}

	var3->setTag("TileEntities", var10);
}

Chunk* ChunkLoader::loadChunkIntoWorldFromCompound(World* var0, NBTTagCompound* var1) {
	int var2 = var1->getInteger("xPos");
	int var3 = var1->getInteger("zPos");
	Chunk* var4 = new Chunk(var0, var2, var3);
	var4->m_blocks = var1->getByteArray("Blocks");
	var4->m_data = new NibbleArray(var1->getByteArray("Data"));
	var4->m_skylightMap = new NibbleArray(var1->getByteArray("SkyLight"));
	var4->m_blocklightMap = new NibbleArray(var1->getByteArray("BlockLight"));
	var4->m_heightMap = var1->getByteArray("HeightMap");
	var4->m_isTerrainPopulated = var1->getBoolean("TerrainPopulated");
	if(!var4->m_data->isValid()) {
		var4->m_data = new NibbleArray(var4->m_blocks_count);
	}

	if(var4->m_heightMap == NULL || !var4->m_skylightMap->isValid()) {
		var4->m_heightMap = new unsigned char[256];
		var4->m_skylightMap = new NibbleArray(var4->m_blocks_count);
		var4->func_1024_c();
	}

	if(!var4->m_blocklightMap->isValid()) {
		var4->m_blocklightMap = new NibbleArray(var4->m_blocks_count);
		var4->func_1014_a();
	}

	NBTTagList* var5 = var1->getTagList("Entities");
	if(var5 != NULL) {
		for(int var6 = 0; var6 < var5->tagCount(); ++var6) {
			NBTTagCompound* var7 = (NBTTagCompound*)var5->tagAt(var6);
			Entity* var8 = EntityList::createEntityFromNBT(var7, var0);
			var4->m_hasEntities = true;
			if(var8 != NULL) {
				var4->addEntity(var8);
			}
		}
	}

	NBTTagList* var10 = var1->getTagList("TileEntities");
	if(var10 != NULL) {
		for(int var11 = 0; var11 < var10->tagCount(); ++var11) {
			NBTTagCompound* var12 = (NBTTagCompound*)var10->tagAt(var11);
			TileEntity* var9 = TileEntity::createAndLoadEntity(var12);
			if(var9 != NULL) {
				var4->func_1001_a(var9);
			}
		}
	}

	return var4;
}

void ChunkLoader::func_814_a() {
	// No-op in Alpha (matches Java).
}

void ChunkLoader::saveExtraData() {
	// No-op in Alpha (matches Java).
}

void ChunkLoader::saveExtraChunkData(World* var1, Chunk* var2) {
	// No-op in Alpha (matches Java).
	(void)var1;
	(void)var2;
}
