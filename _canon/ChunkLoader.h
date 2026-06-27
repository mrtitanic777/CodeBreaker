#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

// Auto-generated from ChunkLoader.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IChunkLoader.h"

class Chunk;
class File;
class NBTTagCompound;
class World;

class ChunkLoader : public IChunkLoader {
public:
	ChunkLoader(File* var1, bool var2);
	virtual ~ChunkLoader();

	File* saveDir;
	bool createIfNecessary;

	virtual File* chunkFileForXZ(int var1, int var2);
	virtual Chunk* loadChunk(World* var1, int var2, int var3);
	virtual void saveChunk(World* var1, Chunk* var2);
	virtual void storeChunkInCompound(Chunk* var1, World* var2, NBTTagCompound* var3);
	static Chunk* loadChunkIntoWorldFromCompound(World* var0, NBTTagCompound* var1);
	virtual void func_814_a();
	virtual void saveExtraData();
	virtual void saveExtraChunkData(World* var1, Chunk* var2);
};

#endif // CHUNKLOADER_H