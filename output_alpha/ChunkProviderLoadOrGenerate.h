#ifndef CHUNKPROVIDERLOADORGENERATE_H
#define CHUNKPROVIDERLOADORGENERATE_H

// Auto-generated from ChunkProviderLoadOrGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IChunkProvider.h"

class Chunk;
class IChunkLoader;
class IProgressUpdate;
class World;

class ChunkProviderLoadOrGenerate : public IChunkProvider {
public:
	ChunkProviderLoadOrGenerate();
	ChunkProviderLoadOrGenerate(World* var1, IChunkLoader* var2, IChunkProvider* var3);
	virtual ~ChunkProviderLoadOrGenerate();

	virtual bool chunkExists(int var1, int var2);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool func_532_a();
	virtual bool func_536_b();

public: // was private
	Chunk* m_blankChunk;
	IChunkProvider* m_chunkProvider;
	IChunkLoader* m_chunkLoader;
	Chunk* m_chunks;
	World* worldObj;
	int m_lastQueriedChunkXPos;
	int m_lastQueriedChunkZPos;
	Chunk* m_lastQueriedChunk;

	Chunk* func_542_c(int var1, int var2);
	void saveExtraChunkData(Chunk* var1);
	void saveChunk(Chunk* var1);

public:  // array-length companions
	int m_chunks_count;

};

#endif // CHUNKPROVIDERLOADORGENERATE_H