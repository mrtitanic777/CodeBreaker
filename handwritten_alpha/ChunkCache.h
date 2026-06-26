#ifndef CHUNKCACHE_H
#define CHUNKCACHE_H

// Auto-generated from ChunkCache.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: m_field_1062_c is a Chunk[][] (2D array of chunk references) ->
// Chunk*** (array of rows, each a Chunk*[] ). Indexed twice it yields a Chunk*.

#include "IBlockAccess.h"

class Chunk;
class Material;
class TileEntity;
class World;
class WorldChunkManager;

class ChunkCache : public IBlockAccess {
public:
	ChunkCache();
	ChunkCache(World* var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual ~ChunkCache();

	virtual int getBlockId(int var1, int var2, int var3);
	virtual TileEntity* getBlockTileEntity(int var1, int var2, int var3);
	virtual float getLightBrightness(int var1, int var2, int var3);
	virtual int func_4086_d(int var1, int var2, int var3);
	virtual int func_716_a(int var1, int var2, int var3, bool var4);
	virtual int getBlockMetadata(int var1, int var2, int var3);
	virtual Material* getBlockMaterial(int var1, int var2, int var3);
	virtual bool isBlockOpaqueCube(int var1, int var2, int var3);
	virtual WorldChunkManager* func_4075_a();

public: // was private
	int m_field_1060_a;
	int m_field_1059_b;
	Chunk*** m_field_1062_c;
	World* worldObj;

public:  // array-length companions
	int m_field_1062_c_count;     // number of rows (X span)
	int m_field_1062_c_inner;     // number of cols (Z span)

};

#endif // CHUNKCACHE_H
