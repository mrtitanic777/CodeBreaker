#ifndef CHUNKCACHE_H
#define CHUNKCACHE_H

// Auto-generated from ChunkCache.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IBlockAccess.h"

class Chunk;
class Material;
class TileEntity;
class World;
class WorldChunkManager;

class ChunkCache : public IBlockAccess {
public:
	ChunkCache(World* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~ChunkCache();

	int chunkX;
	int chunkZ;
	JArray<JArray<Chunk* > > chunkArray;
	World* worldObj;

	virtual int getBlockId(int var1, int var2, int var3);
	virtual TileEntity* getBlockTileEntity(int var1, int var2, int var3);
	virtual float getLightBrightness(int var1, int var2, int var3);
	virtual int getBlockLightValue(int var1, int var2, int var3);
	virtual int addPendingConnection(int var1, int var2, int var3, bool var4);
	virtual int getBlockMetadata(int var1, int var2, int var3);
	virtual Material* getBlockMaterial(int var1, int var2, int var3);
	virtual bool isBlockOpaqueCube(int var1, int var2, int var3);
	virtual WorldChunkManager* getWorldChunkManager();
};

#endif // CHUNKCACHE_H