#ifndef CHUNKPROVIDERGENERATE_H
#define CHUNKPROVIDERGENERATE_H

// Auto-generated from ChunkProviderGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IChunkProvider.h"

class Chunk;
class IProgressUpdate;
class MapGenBase;
class MobSpawnerBase;
class NoiseGeneratorOctaves;
class Random;
class World;
class WorldChunkManager;   // PORT(★ parallel pre-gen): per-worker biome manager (see genWCM below)

class ChunkProviderGenerate : public IChunkProvider {
public:
	ChunkProviderGenerate(World* var1, long long var2);
	virtual ~ChunkProviderGenerate();

	Random* rand;
	NoiseGeneratorOctaves* noiseGen1;
	NoiseGeneratorOctaves* noiseGen2;
	NoiseGeneratorOctaves* noiseGen3;
	NoiseGeneratorOctaves* noiseGen4;
	NoiseGeneratorOctaves* noiseGen5;
	NoiseGeneratorOctaves* noiseGen6;
	NoiseGeneratorOctaves* noiseGen7;
	NoiseGeneratorOctaves* mobSpawnerNoise;
	World* worldObj;
	JArray<double > noiseArray;
	JArray<double > sandNoise;
	JArray<double > gravelNoise;
	JArray<double > stoneNoise;
	MapGenBase* caveGenerator;
	JArray<MobSpawnerBase* > biomesForGeneration;
	JArray<double > noiseData1;
	JArray<double > noiseData2;
	JArray<double > noiseData3;
	JArray<double > noiseData4;
	JArray<double > noiseData5;
	JArray<JArray<int > > field_914_i;
	JArray<double > temperatureField;

	// PORT(★ parallel pre-gen): make provideChunk runnable on a j2x_pool worker with NO shared writes.
	// genWCM != NULL  -> use it for biome lookup instead of worldObj's SHARED WorldChunkManager (whose
	//                    temperature/humidity scratch would race across threads). One WCM per worker.
	// genDeferSpawn    -> skip performWorldGenSpawning (it writes entities to the world); the main thread
	//                    does the spawning after the generated chunks are installed.
	WorldChunkManager* genWCM;
	bool genDeferSpawn;

	virtual void generateTerrain(int var1, int var2, JArray<unsigned char > var3, JArray<MobSpawnerBase* > var4, JArray<double > var5);
	virtual void replaceBlocksForBiome(int var1, int var2, JArray<unsigned char > var3, JArray<MobSpawnerBase* > var4);
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
	ChunkProviderGenerate(const ChunkProviderGenerate&);
	ChunkProviderGenerate& operator=(const ChunkProviderGenerate&);
};

#endif // CHUNKPROVIDERGENERATE_H