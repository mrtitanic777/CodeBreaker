#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from PathEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PathEntity.h"
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
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

PathEntity::PathEntity(JArray<PathPoint* > var1)
	: pathLength(0)
	, pathIndex(0)
{
	this->points = var1;
	this->pathLength = var1.length;
}

PathEntity::~PathEntity()
{
}

void PathEntity::incrementPathIndex()
{
	++this->pathIndex;
}

bool PathEntity::isFinished()
{
	return this->pathIndex >= this->points.length;
}

Vec3D* PathEntity::getPosition(Entity* var1)
{
	double var2 = (double)this->points[this->pathIndex]->xCoord + (double)((int)(var1->width + 1.0F)) * 0.5;
	double var4 = (double)this->points[this->pathIndex]->yCoord;
	double var6 = (double)this->points[this->pathIndex]->zCoord + (double)((int)(var1->width + 1.0F)) * 0.5;
	return Vec3D::createVector(var2, var4, var6);
}

void PathEntity::initStatics()
{
}
