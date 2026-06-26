#ifndef CHUNK_H
#define CHUNK_H

// Auto-generated from Chunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

struct AxisAlignedBB;
class Class;
class Entity;
struct EnumSkyBlock;
class Map;
class NibbleArray;
class Random;
class TileEntity;
class World;

class Chunk {
public:
	Chunk();
	Chunk(World* var1, int var2, int var3);
	Chunk(World* var1, unsigned char* var2, int var3, int var4);
	virtual ~Chunk();

	static bool s_field_1540_a;
	unsigned char* m_blocks;
	bool m_isChunkLoaded;
	World* worldObj;
	NibbleArray* m_data;
	NibbleArray* m_skylightMap;
	NibbleArray* m_blocklightMap;
	unsigned char* m_heightMap;
	int m_field_1532_i;
	int m_xPosition;
	int m_zPosition;
	std::map<std::string, void*>* m_chunkTileEntityMap;
	std::vector<void*>* /* PORT: determine element type */ m_entities;
	bool m_isTerrainPopulated;
	bool m_isModified;
	bool m_neverSave;
	bool m_field_1524_q;
	bool m_hasEntities;
	long long m_lastSaveTime;

	virtual bool isAtLocation(int var1, int var2);
	virtual int getHeightValue(int var1, int var2);
	virtual void func_1014_a();
	virtual void generateHeightMap();
	virtual void func_1024_c();
	virtual void func_4143_d();
	virtual int getBlockID(int var1, int var2, int var3);
	virtual bool setBlockIDWithMetadata(int var1, int var2, int var3, int var4, int var5);
	virtual bool setBlockID(int var1, int var2, int var3, int var4);
	virtual int getBlockMetadata(int var1, int var2, int var3);
	virtual void setBlockMetadata(int var1, int var2, int var3, int var4);
	virtual int getSavedLightValue(EnumSkyBlock* var1, int var2, int var3, int var4);
	virtual void setLightValue(EnumSkyBlock* var1, int var2, int var3, int var4, int var5);
	virtual int getBlockLightValue(int var1, int var2, int var3, int var4);
	virtual void addEntity(Entity* var1);
	virtual void func_1015_b(Entity* var1);
	virtual void func_1016_a(Entity* var1, int var2);
	virtual bool canBlockSeeTheSky(int var1, int var2, int var3);
	virtual TileEntity* getChunkBlockTileEntity(int var1, int var2, int var3);
	virtual void func_1001_a(TileEntity* var1);
	virtual void setChunkBlockTileEntity(int var1, int var2, int var3, TileEntity* var4);
	virtual void removeChunkBlockTileEntity(int var1, int var2, int var3);
	virtual void onChunkLoad();
	virtual void onChunkUnload();
	virtual void setChunkModified();
	virtual void getEntitiesWithinAABBForEntity(Entity* var1, AxisAlignedBB var2, std::vector<void*> /* PORT: determine element type */ var3);
	virtual void getEntitiesOfTypeWithinAAAB(void* var1, AxisAlignedBB var2, std::vector<void*> /* PORT: determine element type */ var3);
	virtual bool needsSaving(bool var1);
	virtual int func_1004_a(unsigned char* var1, int var2, int var3, int var4, int var5, int var6, int var7, int var8);
	virtual Random* func_997_a(long long var1);

public: // was private
	void func_996_c(int var1, int var2);
	void func_1020_f(int var1, int var2, int var3);
	void func_1003_g(int var1, int var2, int var3);

public:  // array-length companions
	int m_blocks_count;
	int m_heightMap_count;
	int m_entities_count;

};

#endif // CHUNK_H