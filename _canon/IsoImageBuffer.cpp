#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from IsoImageBuffer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IsoImageBuffer.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
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

IsoImageBuffer::IsoImageBuffer(World* var1, int var2, int var3)
	: image(NULL)
	, worldObj(NULL)
	, anotherInt(0)
	, anInt(0)
	, isDirty(0)
	, isUpdating(0)
	, counter(0)
	, isLoaded(0)
{
	this->isDirty = false;
	this->isUpdating = false;
	this->counter = 0;
	this->isLoaded = false;
	this->worldObj = var1;
	this->resetData(var2, var3);
}

IsoImageBuffer::~IsoImageBuffer()
{
}

void IsoImageBuffer::resetData(int var1, int var2)
{
	this->isDirty = false;
	this->anotherInt = var1;
	this->anInt = var2;
	this->counter = 0;
	this->isLoaded = false;
}

void IsoImageBuffer::setup(World* var1, int var2, int var3)
{
	this->worldObj = var1;
	this->resetData(var2, var3);
}

void IsoImageBuffer::initStatics()
{
}
