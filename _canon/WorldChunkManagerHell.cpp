#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldChunkManagerHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldChunkManagerHell.h"
#include "ChunkCoordIntPair.h"
#include "Class.h"
#include "MobSpawnerBase.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "WorldChunkManager.h"

WorldChunkManagerHell::WorldChunkManagerHell(MobSpawnerBase* var1, double var2, double var4)
	: biome(NULL)
	, fixedTemperature(0)
	, fixedHumidity(0)
{
	this->biome = var1;
	this->fixedTemperature = var2;
	this->fixedHumidity = var4;
}

WorldChunkManagerHell::~WorldChunkManagerHell()
{
}

MobSpawnerBase* WorldChunkManagerHell::getBiomeForChunk(ChunkCoordIntPair* var1)
{
	return this->biome;
}

MobSpawnerBase* WorldChunkManagerHell::getBiome(int var1, int var2)
{
	return this->biome;
}

double WorldChunkManagerHell::getTemperature(int var1, int var2)
{
	return this->fixedTemperature;
}

JArray<MobSpawnerBase* > WorldChunkManagerHell::getBiomesGrid(int var1, int var2, int var3, int var4)
{
	this->biomeCache = this->loadBlockGeneratorData(this->biomeCache, var1, var2, var3, var4);
	return this->biomeCache;
}

JArray<double > WorldChunkManagerHell::getTemperatures(JArray<double > var1, int var2, int var3, int var4, int var5)
{
	if(var1 == NULL || var1.length < var4 * var5) {
	var1 = JArray<double >(var4 * var5);
	}
	Arrays::fill(var1, 0, var4 * var5, this->fixedTemperature);
	return var1;
}

JArray<MobSpawnerBase* > WorldChunkManagerHell::loadBlockGeneratorData(JArray<MobSpawnerBase* > var1, int var2, int var3, int var4, int var5)
{
	if(var1 == NULL || var1.length < var4 * var5) {
	var1 = JArray<MobSpawnerBase* >(var4 * var5);
	this->temperature = JArray<double >(var4 * var5);
	this->humidity = JArray<double >(var4 * var5);
	}
	Arrays::fill(var1, 0, var4 * var5, this->biome);
	Arrays::fill(this->humidity, 0, var4 * var5, this->fixedHumidity);
	Arrays::fill(this->temperature, 0, var4 * var5, this->fixedTemperature);
	return var1;
}

void WorldChunkManagerHell::initStatics()
{
}
