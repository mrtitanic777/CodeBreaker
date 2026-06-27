#ifndef CHUNKPROVIDERLOADORGENERATE_H
#define CHUNKPROVIDERLOADORGENERATE_H

// Auto-generated from ChunkProviderLoadOrGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IChunkProvider.h"

class Chunk;
class IChunkLoader;
class IProgressUpdate;
class World;

class ChunkProviderLoadOrGenerate : public IChunkProvider {
public:
	ChunkProviderLoadOrGenerate(World* var1, IChunkLoader* var2, IChunkProvider* var3);
	virtual ~ChunkProviderLoadOrGenerate();

	Chunk* blankChunk;
	IChunkProvider* chunkProvider;
	IChunkLoader* chunkLoader;
	JArray<Chunk* > chunks;
	World* worldObj;
	int lastQueriedChunkXPos;
	int lastQueriedChunkZPos;
	Chunk* lastQueriedChunk;

	virtual bool chunkExists(int var1, int var2);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual Chunk* loadChunk(int var1, int var2);
	virtual void saveExtraChunkData(Chunk* var1);
	virtual void saveChunk(Chunk* var1);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool unloadQueuedChunks();
	virtual bool canSave();

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ChunkProviderLoadOrGenerate(const ChunkProviderLoadOrGenerate&);
	ChunkProviderLoadOrGenerate& operator=(const ChunkProviderLoadOrGenerate&);
};

#endif // CHUNKPROVIDERLOADORGENERATE_H