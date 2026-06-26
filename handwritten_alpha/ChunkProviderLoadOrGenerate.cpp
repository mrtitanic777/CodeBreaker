#include "stdafx.h"
// Hand-ported from ChunkProviderLoadOrGenerate.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// The client-side chunk cache: a fixed 32x32 (=1024) ring of loaded Chunks
// indexed by (chunkX & 31) + (chunkZ & 31) * 32. On a miss it asks the
// IChunkLoader to read the chunk from disk; failing that it delegates to the
// inner IChunkProvider (the world generator) -- or hands back the shared blank
// chunk when there is no generator. Loaded chunks are populated once their
// 2x2 neighbourhood is resident, and flushed back through the loader on save.
//
// PORT NOTE (m_chunks): the Java field is `Chunk[] m_chunks` -- an array of 1024
// nullable Chunk *references*, where slots are reassigned, compared by identity
// (== m_blankChunk) and nulled. The auto-generated header mistyped it as a flat
// `Chunk* m_chunks` (a `Chunk[1024]` value array via `new Chunk[1024]`), which
// cannot hold NULL, swap objects, or carry identity. We may NOT edit the header
// (callers link against it as-is), so -- exactly like the already-ported
// MCHashTable.cpp does for its dropped transient fields -- the real
// `Chunk*[1024]` pointer cache lives in a file-local side-table keyed by the
// instance `this` pointer. ChunkProviderLoadOrGenerate is always heap-allocated
// (World owns it via `new`), so `this` is stable for the object's lifetime.

#include "ChunkProviderLoadOrGenerate.h"
#include "Common.h"
#include "Chunk.h"
#include "IChunkLoader.h"
#include "IProgressUpdate.h"
#include "World.h"
#include "IChunkProvider.h"
#include <map>

// ----------------------------------------------------------------------------
// Per-instance chunk-pointer cache (the mistyped m_chunks lives here).
// ----------------------------------------------------------------------------
namespace {
	const int CHUNK_CACHE_LEN = 1024; // 32 * 32

	struct ChunkCache {
		Chunk* slots[CHUNK_CACHE_LEN];

		ChunkCache() {
			for (int i = 0; i < CHUNK_CACHE_LEN; ++i) {
				slots[i] = NULL;
			}
		}
	};

	std::map<const ChunkProviderLoadOrGenerate*, ChunkCache*> g_caches;

	ChunkCache* cacheFor(const ChunkProviderLoadOrGenerate* self) {
		std::map<const ChunkProviderLoadOrGenerate*, ChunkCache*>::iterator it = g_caches.find(self);
		if (it != g_caches.end()) {
			return it->second;
		}
		// Defensive: ctor inserts, but never return NULL.
		ChunkCache* c = new ChunkCache();
		g_caches[self] = c;
		return c;
	}
}

// ----------------------------------------------------------------------------
// Construction / destruction
// ----------------------------------------------------------------------------
ChunkProviderLoadOrGenerate::ChunkProviderLoadOrGenerate()
	: m_blankChunk(NULL)
	, m_chunkProvider(NULL)
	, m_chunkLoader(NULL)
	, m_chunks(NULL)
	, worldObj(NULL)
	, m_lastQueriedChunkXPos(-999999999)
	, m_lastQueriedChunkZPos(-999999999)
	, m_lastQueriedChunk(NULL)
	, m_chunks_count(CHUNK_CACHE_LEN)
{
	g_caches[this] = new ChunkCache();
}

ChunkProviderLoadOrGenerate::ChunkProviderLoadOrGenerate(World* var1, IChunkLoader* var2, IChunkProvider* var3)
	: m_blankChunk(NULL)
	, m_chunkProvider(NULL)
	, m_chunkLoader(NULL)
	, m_chunks(NULL)
	, worldObj(NULL)
	, m_lastQueriedChunkXPos(-999999999)
	, m_lastQueriedChunkZPos(-999999999)
	, m_lastQueriedChunk(NULL)
	, m_chunks_count(CHUNK_CACHE_LEN)
{
	g_caches[this] = new ChunkCache();

	// Java: new Chunk(var1, new byte[32768], 0, 0)  (16*16*128 block array)
	m_blankChunk = new Chunk(var1, new unsigned char[32768], 0, 0);
	m_blankChunk->m_field_1524_q = true;
	m_blankChunk->m_neverSave = true;
	worldObj = var1;
	m_chunkLoader = var2;
	m_chunkProvider = var3;
}

ChunkProviderLoadOrGenerate::~ChunkProviderLoadOrGenerate() {
	std::map<const ChunkProviderLoadOrGenerate*, ChunkCache*>::iterator it = g_caches.find(this);
	if (it != g_caches.end()) {
		delete it->second;
		g_caches.erase(it);
	}

	m_blankChunk = NULL;
	m_chunkProvider = NULL;
	m_chunkLoader = NULL;
	m_chunks = NULL;
	worldObj = NULL;
	m_lastQueriedChunk = NULL;
}

// ----------------------------------------------------------------------------
// IChunkProvider implementation
// ----------------------------------------------------------------------------
bool ChunkProviderLoadOrGenerate::chunkExists(int var1, int var2) {
	if (var1 == m_lastQueriedChunkXPos && var2 == m_lastQueriedChunkZPos && m_lastQueriedChunk != NULL) {
		return true;
	} else {
		ChunkCache* cache = cacheFor(this);
		int var3 = var1 & 31;
		int var4 = var2 & 31;
		int var5 = var3 + var4 * 32;
		return cache->slots[var5] != NULL && (cache->slots[var5] == m_blankChunk || cache->slots[var5]->isAtLocation(var1, var2));
	}
}

Chunk* ChunkProviderLoadOrGenerate::provideChunk(int var1, int var2) {
	if (var1 == m_lastQueriedChunkXPos && var2 == m_lastQueriedChunkZPos && m_lastQueriedChunk != NULL) {
		return m_lastQueriedChunk;
	} else {
		ChunkCache* cache = cacheFor(this);
		int var3 = var1 & 31;
		int var4 = var2 & 31;
		int var5 = var3 + var4 * 32;
		if (!chunkExists(var1, var2)) {
			if (cache->slots[var5] != NULL) {
				cache->slots[var5]->onChunkUnload();
				saveChunk(cache->slots[var5]);
				saveExtraChunkData(cache->slots[var5]);
			}

			Chunk* var6 = func_542_c(var1, var2);
			if (var6 == NULL) {
				if (m_chunkProvider == NULL) {
					var6 = m_blankChunk;
				} else {
					var6 = m_chunkProvider->provideChunk(var1, var2);
				}
			}

			cache->slots[var5] = var6;
			if (var6 != NULL) {
				var6->func_4143_d();
			}

			if (cache->slots[var5] != NULL) {
				cache->slots[var5]->onChunkLoad();
			}

			if (!cache->slots[var5]->m_isTerrainPopulated && chunkExists(var1 + 1, var2 + 1) && chunkExists(var1, var2 + 1) && chunkExists(var1 + 1, var2)) {
				populate(this, var1, var2);
			}

			if (chunkExists(var1 - 1, var2) && !provideChunk(var1 - 1, var2)->m_isTerrainPopulated && chunkExists(var1 - 1, var2 + 1) && chunkExists(var1, var2 + 1) && chunkExists(var1 - 1, var2)) {
				populate(this, var1 - 1, var2);
			}

			if (chunkExists(var1, var2 - 1) && !provideChunk(var1, var2 - 1)->m_isTerrainPopulated && chunkExists(var1 + 1, var2 - 1) && chunkExists(var1, var2 - 1) && chunkExists(var1 + 1, var2)) {
				populate(this, var1, var2 - 1);
			}

			if (chunkExists(var1 - 1, var2 - 1) && !provideChunk(var1 - 1, var2 - 1)->m_isTerrainPopulated && chunkExists(var1 - 1, var2 - 1) && chunkExists(var1, var2 - 1) && chunkExists(var1 - 1, var2)) {
				populate(this, var1 - 1, var2 - 1);
			}
		}

		m_lastQueriedChunkXPos = var1;
		m_lastQueriedChunkZPos = var2;
		m_lastQueriedChunk = cache->slots[var5];
		return cache->slots[var5];
	}
}

Chunk* ChunkProviderLoadOrGenerate::func_542_c(int var1, int var2) {
	if (m_chunkLoader == NULL) {
		return NULL;
	} else {
		// Java wrapped this in try/catch(Exception); the loader port returns NULL
		// on failure instead of throwing, so no guard is needed here.
		Chunk* var3 = m_chunkLoader->loadChunk(worldObj, var1, var2);
		if (var3 != NULL) {
			var3->m_lastSaveTime = worldObj->m_worldTime;
		}

		return var3;
	}
}

void ChunkProviderLoadOrGenerate::saveExtraChunkData(Chunk* var1) {
	if (m_chunkLoader != NULL) {
		// Java: try { ... } catch(Exception) { printStackTrace }
		m_chunkLoader->saveExtraChunkData(worldObj, var1);
	}
}

void ChunkProviderLoadOrGenerate::saveChunk(Chunk* var1) {
	if (m_chunkLoader != NULL) {
		// Java: try { ... } catch(IOException) { printStackTrace }
		var1->m_lastSaveTime = worldObj->m_worldTime;
		m_chunkLoader->saveChunk(worldObj, var1);
	}
}

void ChunkProviderLoadOrGenerate::populate(IChunkProvider* var1, int var2, int var3) {
	Chunk* var4 = provideChunk(var2, var3);
	if (!var4->m_isTerrainPopulated) {
		var4->m_isTerrainPopulated = true;
		if (m_chunkProvider != NULL) {
			m_chunkProvider->populate(var1, var2, var3);
			var4->setChunkModified();
		}
	}
}

bool ChunkProviderLoadOrGenerate::saveChunks(bool var1, IProgressUpdate* var2) {
	ChunkCache* cache = cacheFor(this);
	int var3 = 0;
	int var4 = 0;
	int var5;
	if (var2 != NULL) {
		for (var5 = 0; var5 < m_chunks_count; ++var5) {
			if (cache->slots[var5] != NULL && cache->slots[var5]->needsSaving(var1)) {
				++var4;
			}
		}
	}

	var5 = 0;

	for (int var6 = 0; var6 < m_chunks_count; ++var6) {
		if (cache->slots[var6] != NULL) {
			if (var1 && !cache->slots[var6]->m_neverSave) {
				saveExtraChunkData(cache->slots[var6]);
			}

			if (cache->slots[var6]->needsSaving(var1)) {
				saveChunk(cache->slots[var6]);
				cache->slots[var6]->m_isModified = false;
				++var3;
				if (var3 == 2 && !var1) {
					return false;
				}

				if (var2 != NULL) {
					++var5;
					if (var5 % 10 == 0) {
						var2->setLoadingProgress(var5 * 100 / var4);
					}
				}
			}
		}
	}

	if (var1) {
		if (m_chunkLoader == NULL) {
			return true;
		}

		m_chunkLoader->saveExtraData();
	}

	return true;
}

bool ChunkProviderLoadOrGenerate::func_532_a() {
	if (m_chunkLoader != NULL) {
		m_chunkLoader->func_814_a();
	}

	return m_chunkProvider->func_532_a();
}

bool ChunkProviderLoadOrGenerate::func_536_b() {
	return true;
}
