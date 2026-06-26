#ifndef CHUNKPROVIDERISO_H
#define CHUNKPROVIDERISO_H

// Auto-generated from ChunkProviderIso.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: m_chunks is a Chunk[] (array of chunk references) -> Chunk**.

#include "IChunkProvider.h"

class Chunk;
class IChunkLoader;
class IProgressUpdate;
class World;

class ChunkProviderIso : public IChunkProvider {
public:
	ChunkProviderIso();
	ChunkProviderIso(World* var1, IChunkLoader* var2);
	virtual ~ChunkProviderIso();

	virtual bool chunkExists(int var1, int var2);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool func_532_a();
	virtual bool func_536_b();

public: // was private
	Chunk** m_chunks;
	World* worldObj;
	IChunkLoader* m_chunkLoader;
	unsigned char* m_field_899_a;

	Chunk* func_543_c(int var1, int var2);

public:  // array-length companions
	int m_chunks_count;
	int m_field_899_a_count;

};

#endif // CHUNKPROVIDERISO_H
