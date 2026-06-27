#ifndef CHUNKPROVIDERHELL_H
#define CHUNKPROVIDERHELL_H

// Auto-generated from ChunkProviderHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IChunkProvider.h"

class Chunk;
class IProgressUpdate;
class MapGenBase;
class NoiseGeneratorOctaves;
class Random;
class World;

class ChunkProviderHell : public IChunkProvider {
public:
	ChunkProviderHell(World* var1, long long var2);
	virtual ~ChunkProviderHell();

	Random* rand;
	NoiseGeneratorOctaves* noiseGen1;
	NoiseGeneratorOctaves* noiseGen2;
	NoiseGeneratorOctaves* noiseGen3;
	NoiseGeneratorOctaves* noiseGen5;
	NoiseGeneratorOctaves* noiseGen6;
	NoiseGeneratorOctaves* noiseGenA;
	NoiseGeneratorOctaves* noiseGenB;
	World* worldObj;
	JArray<double > noiseArray;
	JArray<double > noiseData2;
	JArray<double > noiseData3;
	JArray<double > noiseData4;
	MapGenBase* caveGenerator;
	JArray<double > noiseData5;
	JArray<double > noiseData6;
	JArray<double > noiseData7;
	JArray<double > noiseDataA;
	JArray<double > noiseDataB;

	virtual void generateTerrain(int var1, int var2, JArray<unsigned char > var3);
	virtual void replaceBlocksForBiome(int var1, int var2, JArray<unsigned char > var3);
	virtual Chunk* provideChunk(int var1, int var2);
	virtual JArray<double > initializeNoiseField(JArray<double > var1, int var2, int var3, int var4, int var5, int var6, int var7);
	virtual bool chunkExists(int var1, int var2);
	virtual void populate(IChunkProvider* var1, int var2, int var3);
	virtual bool saveChunks(bool var1, IProgressUpdate* var2);
	virtual bool unloadQueuedChunks();
	virtual bool canSave();

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	ChunkProviderHell(const ChunkProviderHell&);
	ChunkProviderHell& operator=(const ChunkProviderHell&);
};

#endif // CHUNKPROVIDERHELL_H