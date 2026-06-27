#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldChunkManager.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldChunkManager.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkCoordIntPair.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldProvider.h"

WorldChunkManager::WorldChunkManager()
	: temperatureNoise(NULL)
	, humidityNoise(NULL)
	, noiseGen3(NULL)
{
}

WorldChunkManager::WorldChunkManager(World* var1)
	: temperatureNoise(NULL)
	, humidityNoise(NULL)
	, noiseGen3(NULL)
{
	this->temperatureNoise = new NoiseGeneratorOctaves2(new Random(var1->randomSeed * 9871LL), 4);
	this->humidityNoise = new NoiseGeneratorOctaves2(new Random(var1->randomSeed * 39811LL), 4);
	this->noiseGen3 = new NoiseGeneratorOctaves2(new Random(var1->randomSeed * 543321LL), 2);
}

WorldChunkManager::~WorldChunkManager()
{
	delete humidityNoise;
	humidityNoise = NULL;
	delete noiseGen3;
	noiseGen3 = NULL;
	delete temperatureNoise;
	temperatureNoise = NULL;
}

MobSpawnerBase* WorldChunkManager::getBiomeForChunk(ChunkCoordIntPair* var1)
{
	return this->getBiome(var1->chunkXPos, var1->chunkZPos);
}

MobSpawnerBase* WorldChunkManager::getBiome(int var1, int var2)
{
	return this->getBiomesGrid(var1, var2, 1, 1)[0];
}

double WorldChunkManager::getTemperature(int var1, int var2)
{
	this->temperature = this->temperatureNoise->generateNoiseOctaves(this->temperature, (double)var1, (double)var2, 1, 1, (double)0.025F, (double)0.025F, 0.5);
	return this->temperature[0];
}

JArray<MobSpawnerBase* > WorldChunkManager::getBiomesGrid(int var1, int var2, int var3, int var4)
{
	this->biomeCache = this->loadBlockGeneratorData(this->biomeCache, var1, var2, var3, var4);
	return this->biomeCache;
}

JArray<double > WorldChunkManager::getTemperatures(JArray<double > var1, int var2, int var3, int var4, int var5)
{
	if(var1 == NULL || var1.length < var4 * var5) {
	var1 = JArray<double >(var4 * var5);
	}
	var1 = this->temperatureNoise->generateNoiseOctaves(var1, (double)var2, (double)var3, var4, var4, (double)0.025F, (double)0.025F, 0.25);
	this->noiseArray = this->noiseGen3->generateNoiseOctaves(this->noiseArray, (double)var2, (double)var3, var4, var4, 0.25, 0.25, 0.5882352941176471);
	int var6 = 0;
	for(int var7 = 0; var7 < var4; ++var7) {
	for(int var8 = 0; var8 < var5; ++var8) {
	double var9 = this->noiseArray[var6] * 1.1 + 0.5;
	double var11 = 0.01;
	double var13 = 1.0 - var11;
	double var15 = (var1[var6] * 0.15 + 0.7) * var13 + var9 * var11;
	var15 = 1.0 - (1.0 - var15) * (1.0 - var15);
	if(var15 < 0.0) {
	var15 = 0.0;
	}
	if(var15 > 1.0) {
	var15 = 1.0;
	}
	var1[var6] = var15;
	++var6;
	}
	}
	return var1;
}

JArray<MobSpawnerBase* > WorldChunkManager::loadBlockGeneratorData(JArray<MobSpawnerBase* > var1, int var2, int var3, int var4, int var5)
{
	if(var1 == NULL || var1.length < var4 * var5) {
	var1 = JArray<MobSpawnerBase* >(var4 * var5);
	}
	this->temperature = this->temperatureNoise->generateNoiseOctaves(this->temperature, (double)var2, (double)var3, var4, var4, (double)0.025F, (double)0.025F, 0.25);
	this->humidity = this->humidityNoise->generateNoiseOctaves(this->humidity, (double)var2, (double)var3, var4, var4, (double)0.05F, (double)0.05F, 1.0 / 3.0);
	this->noiseArray = this->noiseGen3->generateNoiseOctaves(this->noiseArray, (double)var2, (double)var3, var4, var4, 0.25, 0.25, 0.5882352941176471);
	int var6 = 0;
	for(int var7 = 0; var7 < var4; ++var7) {
	for(int var8 = 0; var8 < var5; ++var8) {
	double var9 = this->noiseArray[var6] * 1.1 + 0.5;
	double var11 = 0.01;
	double var13 = 1.0 - var11;
	double var15 = (this->temperature[var6] * 0.15 + 0.7) * var13 + var9 * var11;
	var11 = 0.002;
	var13 = 1.0 - var11;
	double var17 = (this->humidity[var6] * 0.15 + 0.5) * var13 + var9 * var11;
	var15 = 1.0 - (1.0 - var15) * (1.0 - var15);
	if(var15 < 0.0) {
	var15 = 0.0;
	}
	if(var17 < 0.0) {
	var17 = 0.0;
	}
	if(var15 > 1.0) {
	var15 = 1.0;
	}
	if(var17 > 1.0) {
	var17 = 1.0;
	}
	this->temperature[var6] = var15;
	this->humidity[var6] = var17;
	var1[var6++] = MobSpawnerBase::getBiomeFromLookup(var15, var17);
	}
	}
	return var1;
}

void WorldChunkManager::initStatics()
{
}
