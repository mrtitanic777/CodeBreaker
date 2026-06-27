#ifndef CHUNKPROVIDERISO_H
#define CHUNKPROVIDERISO_H

// Auto-generated from ChunkProviderIso.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IChunkProvider.h"

class Chunk;
class IChunkLoader;
class IProgressUpdate;
class World;

class ChunkProviderIso : public IChunkProvider {
public:
	ChunkProviderIso(World* var1, IChunkLoader* var2);
	virtual ~ChunkProviderIso();

	JArray<Chunk* > chunks;
	World* worldObj;
	IChunkLoader* chunkLoader;
	JArray<unsigned char > blankBlocks;

	virtual bool chunkExists(int var1, int var2);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual Chunk* func_543_c(int var1, int var2);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool unloadQueuedChunks();
	virtual bool canSave();

	static void initStatics();
};

#endif // CHUNKPROVIDERISO_H