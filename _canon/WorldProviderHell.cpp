#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldProviderHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldProviderHell.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkLoader.h"
#include "ChunkProviderHell.h"
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
#include "WorldChunkManagerHell.h"
#include "WorldProvider.h"

WorldProviderHell::~WorldProviderHell()
{
}

void WorldProviderHell::registerWorldChunkManager()
{
	this->worldChunkMgr = new WorldChunkManagerHell(MobSpawnerBase::s_hell, 1.0, 0.0);
	this->isHellWorld = true;
	this->doesWaterVaporize = true;
	this->hasNoSky = true;
	this->dimensionId = -1;
}

Vec3D* WorldProviderHell::getFogColor(float var1, float var2)
{
	return Vec3D::createVector((double)0.2F, (double)0.03F, (double)0.03F);
}

void WorldProviderHell::generateLightBrightnessTable()
{
	float var1 = 0.1F;
	for(int var2 = 0; var2 <= 15; ++var2) {
	float var3 = 1.0F - (float)var2 / 15.0F;
	this->lightBrightnessTable[var2] = (1.0F - var3) / (var3 * 3.0F + 1.0F) * (1.0F - var1) + var1;
	}
}

IChunkProvider* WorldProviderHell::getChunkProvider()
{
	return new ChunkProviderHell(this->worldObj, this->worldObj->randomSeed);
}

IChunkLoader* WorldProviderHell::getChunkLoader(File* var1)
{
	File* var2 = new File(var1, "DIM-1");
	var2->mkdirs();
	return new ChunkLoader(var2, true);
}

bool WorldProviderHell::canCoordinateBeSpawn(int var1, int var2)
{
	int var3 = this->worldObj->getTopBlockId(var1, var2);
	return var3 == Block::s_bedrock->blockID ? false : (var3 == 0 ? false : Block::s_opaqueCubeLookup[var3]);
}

float WorldProviderHell::calculateCelestialAngle(long long var1, float var3)
{
	return 0.5F;
}

bool WorldProviderHell::isSurfaceWorld()
{
	return false;
}

void WorldProviderHell::initStatics()
{
}
