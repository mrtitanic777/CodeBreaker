#ifndef CHUNKPROVIDERCLIENT_H
#define CHUNKPROVIDERCLIENT_H

// Auto-generated from ChunkProviderClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IChunkProvider.h"
#include <vector>

class Chunk;
class IProgressUpdate;
class Map;
class World;

class ChunkProviderClient : public IChunkProvider {
public:
	ChunkProviderClient();
	ChunkProviderClient(World* var1);
	virtual ~ChunkProviderClient();

	virtual bool chunkExists(int var1, int var2);
	virtual void func_539_c(int var1, int var2);
	virtual Chunk* func_538_d(int var1, int var2);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool func_532_a();
	virtual bool func_536_b();
	virtual void populate(IChunkProvider* var1, int var2, int var3);

public: // was private
	Chunk* m_blankChunk;
	std::map<std::string, void*>* m_chunkMapping;
	std::vector<void*> /* PORT: determine element type */ m_unusedChunkList;
	World* worldObj;

};

#endif // CHUNKPROVIDERCLIENT_H