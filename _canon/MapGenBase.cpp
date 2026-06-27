#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MapGenBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MapGenBase.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
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
#include "WorldChunkManager.h"
#include "WorldProvider.h"

MapGenBase::MapGenBase()
	: range(0)
	, rand(NULL)
{
	this->range = 8;
	this->rand = new Random();
}

MapGenBase::~MapGenBase()
{
}

void MapGenBase::generate(IChunkProvider* var1, World* var2, int var3, int var4, JArray<unsigned char > var5)
{
	int var6 = this->range;
	this->rand->setSeed(var2->randomSeed);
	long long var7 = this->rand->nextLong() / 2LL * 2LL + 1LL;
	long long var9 = this->rand->nextLong() / 2LL * 2LL + 1LL;
	for(int var11 = var3 - var6; var11 <= var3 + var6; ++var11) {
	for(int var12 = var4 - var6; var12 <= var4 + var6; ++var12) {
	this->rand->setSeed((long long)var11 * var7 + (long long)var12 * var9 ^ var2->randomSeed);
	this->recursiveGenerate(var2, var11, var12, var3, var4, var5);
	}
	}
}

void MapGenBase::recursiveGenerate(World* var1, int var2, int var3, int var4, int var5, JArray<unsigned char > var6)
{
}

void MapGenBase::initStatics()
{
}
