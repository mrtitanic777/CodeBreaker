#include "stdafx.h"
// Auto-generated from ChunkProviderIso.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: m_chunks is Chunk** (Chunk[] of references). Allocate as Chunk*[256]
// and zero-init (Java new Chunk[256] is null-filled). Cell access uses -> (it is a Chunk*).

#include "ChunkProviderIso.h"
#include "Common.h"
#include "Chunk.h"
#include "IChunkLoader.h"
#include "IProgressUpdate.h"
#include "World.h"
#include "IChunkProvider.h"

ChunkProviderIso::ChunkProviderIso()
	: m_chunks(new Chunk*[256])
	, worldObj(NULL)
	, m_chunkLoader(NULL)
	, m_field_899_a(new unsigned char[-(-32768)])
{
	for(int _i = 0; _i < 256; ++_i) m_chunks[_i] = NULL;
}

ChunkProviderIso::ChunkProviderIso(World* var1, IChunkLoader* var2)
	: m_chunks(new Chunk*[256])
	, worldObj(NULL)
	, m_chunkLoader(NULL)
	, m_field_899_a(new unsigned char[-(-32768)])
{
		for(int _i = 0; _i < 256; ++_i) m_chunks[_i] = NULL;
		worldObj = var1;
		m_chunkLoader = var2;
}

ChunkProviderIso::~ChunkProviderIso() {
	delete[] m_chunks;
	m_chunks = NULL;
	worldObj = NULL;
	m_chunkLoader = NULL;
	delete[] m_field_899_a;
	m_field_899_a = NULL;
}

bool ChunkProviderIso::chunkExists(int var1, int var2) {
		int var3 = var1 & 15 | (var2 & 15) * 16;
		return m_chunks[var3] != NULL && m_chunks[var3]->isAtLocation(var1, var2);
}

Chunk* ChunkProviderIso::provideChunk(int var1, int var2) {
		int var3 = var1 & 15 | (var2 & 15) * 16;

  { // was try
			if(!chunkExists(var1, var2)) {
				Chunk* var4 = func_543_c(var1, var2);
				if(var4 == NULL) {
					var4 = new Chunk(worldObj, m_field_899_a, var1, var2);
					var4->m_field_1524_q = true;
					var4->m_neverSave = true;
				}

				m_chunks[var3] = var4;
			}

			return m_chunks[var3];
		} if(false) { // was catch(Exception var5)
   // Removed Java stream/exception call: var5.printStackTrace();
			return NULL;
		}
}

Chunk* ChunkProviderIso::func_543_c(int var1, int var2) {
  { // was try
			return m_chunkLoader->loadChunk(worldObj, var1, var2);
		} if(false) { // was catch(IOException var4)
   // Removed Java stream/exception call: var4.printStackTrace();
			return NULL;
		}
}

void ChunkProviderIso::populate(IChunkProvider* var1, int var2, int var3) {
	// TODO: implement
}

bool ChunkProviderIso::saveChunks(bool var1, IProgressUpdate* var2) {
		return true;
}

bool ChunkProviderIso::func_532_a() {
		return false;
}

bool ChunkProviderIso::func_536_b() {
		return false;
}
