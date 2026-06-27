#ifndef WORLDCHUNKMANAGER_H
#define WORLDCHUNKMANAGER_H

// Auto-generated from WorldChunkManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ChunkCoordIntPair;
class MobSpawnerBase;
class NoiseGeneratorOctaves2;
class World;

class WorldChunkManager {
public:
	WorldChunkManager();
	WorldChunkManager(World* var1);
	virtual ~WorldChunkManager();

	NoiseGeneratorOctaves2* temperatureNoise;
	NoiseGeneratorOctaves2* humidityNoise;
	NoiseGeneratorOctaves2* noiseGen3;
	JArray<double > temperature;
	JArray<double > humidity;
	JArray<double > noiseArray;
	JArray<MobSpawnerBase* > biomeCache;

	virtual MobSpawnerBase* getBiomeForChunk(ChunkCoordIntPair* var1);
	virtual MobSpawnerBase* getBiome(int var1, int var2);
	virtual double getTemperature(int var1, int var2);
	virtual JArray<MobSpawnerBase* > getBiomesGrid(int var1, int var2, int var3, int var4);
	virtual JArray<double > getTemperatures(JArray<double > var1, int var2, int var3, int var4, int var5);
	virtual JArray<MobSpawnerBase* > loadBlockGeneratorData(JArray<MobSpawnerBase* > var1, int var2, int var3, int var4, int var5);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	WorldChunkManager(const WorldChunkManager&);
	WorldChunkManager& operator=(const WorldChunkManager&);
};

#endif // WORLDCHUNKMANAGER_H