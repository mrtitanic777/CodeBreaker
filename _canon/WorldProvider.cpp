#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldProvider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldProvider.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkLoader.h"
#include "ChunkProviderGenerate.h"
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
#include "MapGenBase.h"
#include "Material.h"
#include "MathHelper.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves.h"
#include "NoiseGeneratorOctaves2.h"
#include "NoiseGeneratorPerlin.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProviderHell.h"

WorldProvider::WorldProvider()
	: worldObj(NULL)
	, worldChunkMgr(NULL)
	, isHellWorld(0)
	, doesWaterVaporize(0)
	, hasNoSky(0)
	, dimensionId(0)
{
	this->isHellWorld = false;
	this->doesWaterVaporize = false;
	this->hasNoSky = false;
	this->lightBrightnessTable = JArray<float >(16);
	this->dimensionId = 0;
	this->colorsSunriseSunset = JArray<float >(4);
}

WorldProvider::~WorldProvider()
{
}

void WorldProvider::registerWorld(World* var1)
{
	this->worldObj = var1;
	this->registerWorldChunkManager();
	this->generateLightBrightnessTable();
}

void WorldProvider::generateLightBrightnessTable()
{
	float var1 = 0.05F;
	for(int var2 = 0; var2 <= 15; ++var2) {
	float var3 = 1.0F - (float)var2 / 15.0F;
	this->lightBrightnessTable[var2] = (1.0F - var3) / (var3 * 3.0F + 1.0F) * (1.0F - var1) + var1;
	}
}

void WorldProvider::registerWorldChunkManager()
{
	this->worldChunkMgr = new WorldChunkManager(this->worldObj);
}

IChunkProvider* WorldProvider::getChunkProvider()
{
	return new ChunkProviderGenerate(this->worldObj, this->worldObj->randomSeed);
}

IChunkLoader* WorldProvider::getChunkLoader(File* var1)
{
	return new ChunkLoader(var1, true);
}

bool WorldProvider::canCoordinateBeSpawn(int var1, int var2)
{
	int var3 = this->worldObj->getTopBlockId(var1, var2);
	return var3 == Block::s_sand->blockID;
}

float WorldProvider::calculateCelestialAngle(long long var1, float var3)
{
	int var4 = (int)(var1 % 24000LL);
	float var5 = ((float)var4 + var3) / 24000.0F - 0.25F;
	if(var5 < 0.0F) {
	++var5;
	}
	if(var5 > 1.0F) {
	--var5;
	}
	float var6 = var5;
	var5 = 1.0F - (float)((cosf((double)var5 * 3.1415927F) + 1.0) / 2.0);
	var5 = var6 + (var5 - var6) / 3.0F;
	return var5;
}

JArray<float > WorldProvider::calcSunriseSunsetColors(float var1, float var2)
{
	float var3 = 0.4F;
	float var4 = MathHelper::cos(var1 * (float)3.1415927F * 2.0F) - 0.0F;
	float var5 = -0.0F;
	if(var4 >= var5 - var3 && var4 <= var5 + var3) {
	float var6 = (var4 - var5) / var3 * 0.5F + 0.5F;
	float var7 = 1.0F - (1.0F - MathHelper::sin(var6 * (float)3.1415927F)) * 0.99F;
	var7 *= var7;
	this->colorsSunriseSunset[0] = var6 * 0.3F + 0.7F;
	this->colorsSunriseSunset[1] = var6 * var6 * 0.7F + 0.2F;
	this->colorsSunriseSunset[2] = var6 * var6 * 0.0F + 0.2F;
	this->colorsSunriseSunset[3] = var7;
	return this->colorsSunriseSunset;
	} else {
	return JArray<float >();
	}
}

Vec3D* WorldProvider::getFogColor(float var1, float var2)
{
	float var3 = MathHelper::cos(var1 * (float)3.1415927F * 2.0F) * 2.0F + 0.5F;
	if(var3 < 0.0F) {
	var3 = 0.0F;
	}
	if(var3 > 1.0F) {
	var3 = 1.0F;
	}
	float var4 = 192.0F / 255.0F;
	float var5 = 216.0F / 255.0F;
	float var6 = 1.0F;
	var4 *= var3 * 0.94F + 0.06F;
	var5 *= var3 * 0.94F + 0.06F;
	var6 *= var3 * 0.91F + 0.09F;
	return Vec3D::createVector((double)var4, (double)var5, (double)var6);
}

bool WorldProvider::isSurfaceWorld()
{
	return true;
}

WorldProvider* WorldProvider::getProviderForDimension(int var0)
{
	return (WorldProvider*)(var0 == 0 ? new WorldProvider() : (var0 == -1 ? new WorldProviderHell() : NULL));
}

void WorldProvider::initStatics()
{
}
