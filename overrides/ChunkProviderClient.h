#ifndef CHUNKPROVIDERCLIENT_H
#define CHUNKPROVIDERCLIENT_H

// Auto-generated from ChunkProviderClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>
#include "IChunkProvider.h"

class Chunk;
class IProgressUpdate;
class World;

class ChunkProviderClient : public IChunkProvider {
public:
	ChunkProviderClient(World* var1);
	virtual ~ChunkProviderClient();

	Chunk* blankChunk;
	// Java keyed this map on ChunkCoordinates (value-equality of x,z). std::map needs an
	// ordered key, so we pack (x,z) into a long long to preserve exact lookup semantics.
	std::map<long long, void*> chunkMapping;
	JList<void*> unusedChunkList;
	World* worldObj;

	virtual bool chunkExists(int var1, int var2);
	virtual void unloadChunk(int var1, int var2);
	virtual Chunk* loadChunk(int var1, int var2);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool unloadQueuedChunks();
	virtual bool canSave();
	virtual void populate(IChunkProvider* var1, int var2, int var3);

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ChunkProviderClient(const ChunkProviderClient&);
	ChunkProviderClient& operator=(const ChunkProviderClient&);
};

#endif // CHUNKPROVIDERCLIENT_H