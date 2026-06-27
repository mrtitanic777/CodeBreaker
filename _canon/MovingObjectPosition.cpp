#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MovingObjectPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovingObjectPosition.h"
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

MovingObjectPosition::MovingObjectPosition(int var1, int var2, int var3, int var4, Vec3D* var5)
	: typeOfHit(0)
	, blockX(0)
	, blockY(0)
	, blockZ(0)
	, sideHit(0)
	, hitVec(NULL)
	, entityHit(NULL)
{
	this->typeOfHit = 0;
	this->blockX = var1;
	this->blockY = var2;
	this->blockZ = var3;
	this->sideHit = var4;
	this->hitVec = Vec3D::createVector(var5->xCoord, var5->yCoord, var5->zCoord);
}

MovingObjectPosition::MovingObjectPosition(Entity* var1)
	: typeOfHit(0)
	, blockX(0)
	, blockY(0)
	, blockZ(0)
	, sideHit(0)
	, hitVec(NULL)
	, entityHit(NULL)
{
	this->typeOfHit = 1;
	this->entityHit = var1;
	this->hitVec = Vec3D::createVector(var1->posX, var1->posY, var1->posZ);
}

MovingObjectPosition::~MovingObjectPosition()
{
}

void MovingObjectPosition::initStatics()
{
}
