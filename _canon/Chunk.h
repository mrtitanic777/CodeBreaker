#ifndef CHUNK_H
#define CHUNK_H

// Auto-generated from Chunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>
#include "EnumSkyBlock.h"

class AxisAlignedBB;
class Class;
class Entity;
class NibbleArray;
class Random;
class TileEntity;
class World;

class Chunk {
public:
	Chunk(World* var1, int var2, int var3);
	Chunk(World* var1, JArray<unsigned char > var2, int var3, int var4);
	virtual ~Chunk();

	JArray<unsigned char > blocks;
	bool isChunkLoaded;
	World* worldObj;
	NibbleArray* data;
	NibbleArray* skylightMap;
	NibbleArray* blocklightMap;
	JArray<unsigned char > heightMap;
	int heightMapMinimum;
	int xPosition;
	int zPosition;
	// Java key was ChunkPosition (value-equal by x,y,z). Keyed here by the packed
	// coordinate so two distinct ChunkPosition objects with equal coords map to the
	// same entry, preserving Java's equals/hashCode semantics.
	std::map<long long, void*> chunkTileEntityMap;
	JArray<JList<void*> > entities;
	bool isTerrainPopulated;
	bool isModified;
	bool neverSave;
	bool isEmptyChunk;
	bool hasEntities;
	long long lastSaveTime;
	static bool s_isLit;

	virtual bool isAtLocation(int var1, int var2);
	virtual int getHeightValue(int var1, int var2);
	virtual void removeUnknownBlocks();
	virtual void generateHeightMap();
	virtual void generateSkylightMap();
	virtual void generateBlocklightMap();
	virtual void checkSkylightNeighbors(int var1, int var2);
	virtual void updateSkylightNeighborHeight(int var1, int var2, int var3);
	virtual void relightBlock(int var1, int var2, int var3);
	virtual int getBlockID(int var1, int var2, int var3);
	virtual bool setBlockIDWithMetadata(int var1, int var2, int var3, int var4, int var5);
	virtual bool setBlockID(int var1, int var2, int var3, int var4);
	virtual int getBlockMetadata(int var1, int var2, int var3);
	virtual void setBlockMetadata(int var1, int var2, int var3, int var4);
	virtual int getSavedLightValue(int var1, int var2, int var3, int var4);
	virtual void setLightValue(int var1, int var2, int var3, int var4, int var5);
	virtual int getBlockLightValue(int var1, int var2, int var3, int var4);
	virtual void addEntity(Entity* var1);
	virtual void removeEntity(Entity* var1);
	virtual void removeEntityAtIndex(Entity* var1, int var2);
	virtual bool canBlockSeeTheSky(int var1, int var2, int var3);
	virtual TileEntity* getChunkBlockTileEntity(int var1, int var2, int var3);
	virtual void addTileEntity(TileEntity* var1);
	virtual void setChunkBlockTileEntity(int var1, int var2, int var3, TileEntity* var4);
	virtual void removeChunkBlockTileEntity(int var1, int var2, int var3);
	virtual void onChunkLoad();
	virtual void onChunkUnload();
	virtual void setChunkModified();
	virtual void getEntitiesWithinAABBForEntity(Entity* var1, AxisAlignedBB* var2, JList<void*> var3);
	virtual void getEntitiesOfTypeWithinAAAB(Class* var1, AxisAlignedBB* var2, JList<void*> var3);
	virtual bool needsSaving(bool var1);
	virtual int setChunkData(JArray<unsigned char > var1, int var2, int var3, int var4, int var5, int var6, int var7, int var8);
	virtual Random* getRandomWithSeed(long long var1);

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	Chunk(const Chunk&);
	Chunk& operator=(const Chunk&);
};

#endif // CHUNK_H