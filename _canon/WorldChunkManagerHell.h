#ifndef WORLDCHUNKMANAGERHELL_H
#define WORLDCHUNKMANAGERHELL_H

// Auto-generated from WorldChunkManagerHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "WorldChunkManager.h"

class ChunkCoordIntPair;
class MobSpawnerBase;

class WorldChunkManagerHell : public WorldChunkManager {
public:
	WorldChunkManagerHell(MobSpawnerBase* var1, double var2, double var4);
	virtual ~WorldChunkManagerHell();

	MobSpawnerBase* biome;
	double fixedTemperature;
	double fixedHumidity;

	virtual MobSpawnerBase* getBiomeForChunk(ChunkCoordIntPair* var1);
	virtual MobSpawnerBase* getBiome(int var1, int var2);
	virtual double getTemperature(int var1, int var2);
	virtual JArray<MobSpawnerBase* > getBiomesGrid(int var1, int var2, int var3, int var4);
	virtual JArray<double > getTemperatures(JArray<double > var1, int var2, int var3, int var4, int var5);
	virtual JArray<MobSpawnerBase* > loadBlockGeneratorData(JArray<MobSpawnerBase* > var1, int var2, int var3, int var4, int var5);

	static void initStatics();
};

#endif // WORLDCHUNKMANAGERHELL_H