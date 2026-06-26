#include "stdafx.h"
// Auto-generated from ChunkProviderClient.java  (handwritten_alpha override)
// FIXES vs converter output:
//   * m_chunkMapping is std::map<std::string,void*>* (header), but the converter
//     keyed it with ChunkCoordinates* -> key all accesses by an "x,z" string.
//   * map was never allocated (NULL) -> allocate in ctors.
//   * provideChunk() indexed a pointer without deref (m_chunkMapping[k]) -> (*m_chunkMapping).find.
//   * Java Arrays.fill(skylight,-1) -> memset 0xFF over NibbleArray::m_data.
//   * NibbleArray only forward-declared -> include it.
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkProviderClient.h"
#include "Common.h"
#include "Chunk.h"
#include "NibbleArray.h"
#include "IProgressUpdate.h"
#include "World.h"
#include "IChunkProvider.h"
#include <algorithm>
#include <map>
#include <string>
#include <cstdio>
#include <cstring>

// Chunk map key: matches the header's std::string-keyed map. (Java used a long
// LongHashMap key; an "x,z" string is an equivalent unique key for our purposes.)
static std::string chunkKey(int x, int z) {
	char buf[32];
	sprintf(buf, "%d,%d", x, z);
	return std::string(buf);
}

ChunkProviderClient::ChunkProviderClient()
	: m_blankChunk(NULL)
	, m_chunkMapping(new std::map<std::string, void*>())
	, worldObj(NULL)
{
}

ChunkProviderClient::ChunkProviderClient(World* var1)
	: m_blankChunk(NULL)
	, m_chunkMapping(new std::map<std::string, void*>())
	, worldObj(NULL)
{
	m_blankChunk = new Chunk(var1, new unsigned char[32768], 0, 0);
	m_blankChunk->m_field_1524_q = true;
	m_blankChunk->m_neverSave = true;
	worldObj = var1;
}

ChunkProviderClient::~ChunkProviderClient() {
	m_blankChunk = NULL;
	if (m_chunkMapping != NULL) { delete m_chunkMapping; m_chunkMapping = NULL; }
	worldObj = NULL;
}

bool ChunkProviderClient::chunkExists(int var1, int var2) {
	return m_chunkMapping->count(chunkKey(var1, var2)) != 0;
}

void ChunkProviderClient::func_539_c(int var1, int var2) {
	Chunk* var3 = provideChunk(var1, var2);
	if(!var3->m_field_1524_q) {
		var3->onChunkUnload();
	}

	m_chunkMapping->erase(chunkKey(var1, var2));
	std::vector<void*>::iterator _removeIt = std::find(m_unusedChunkList.begin(), m_unusedChunkList.end(), var3);
	if (_removeIt != m_unusedChunkList.end()) m_unusedChunkList.erase(_removeIt);
}

Chunk* ChunkProviderClient::func_538_d(int var1, int var2) {
	unsigned char* var4 = new unsigned char[32768];
	Chunk* var5 = new Chunk(worldObj, var4, var1, var2);
	// Java: Arrays.fill(chunk.skylightMap.data, (byte)-1)  -> full skylight
	if (var5->m_skylightMap != NULL && var5->m_skylightMap->m_data != NULL) {
		memset(var5->m_skylightMap->m_data, 0xFF, (size_t)var5->m_skylightMap->m_data_count);
	}
	(*m_chunkMapping)[chunkKey(var1, var2)] = (void*)(var5);
	var5->m_isChunkLoaded = true;
	return var5;
}

Chunk* ChunkProviderClient::provideChunk(int var1, int var2) {
	std::map<std::string, void*>::iterator it = m_chunkMapping->find(chunkKey(var1, var2));
	Chunk* var4 = (it != m_chunkMapping->end()) ? (Chunk*)it->second : NULL;
	return var4 == NULL ? m_blankChunk : var4;
}

bool ChunkProviderClient::saveChunks(bool var1, IProgressUpdate* var2) {
	return true;
}

bool ChunkProviderClient::func_532_a() {
	return false;
}

bool ChunkProviderClient::func_536_b() {
	return false;
}

void ChunkProviderClient::populate(IChunkProvider* var1, int var2, int var3) {
	// TODO: implement
}
