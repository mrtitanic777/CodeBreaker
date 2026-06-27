#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Pathfinder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Pathfinder.h"
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
#include "HashEntry.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "MCHashTable.h"
#include "Material.h"
#include "MathHelper.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "Path.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

Pathfinder::Pathfinder(IBlockAccess* var1)
	: worldMap(NULL)
	, path(NULL)
	, pointMap(NULL)
{
	this->path = new Path();
	this->pointMap = new MCHashTable();
	this->pathOptions = JArray<PathPoint* >(32);
	this->worldMap = var1;
}

Pathfinder::~Pathfinder()
{
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, Entity* var2, float var3)
{
	return this->createEntityPathTo(var1, var2->posX, var2->boundingBox->minY, var2->posZ, var3);
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, int var2, int var3, int var4, float var5)
{
	return this->createEntityPathTo(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), var5);
}

PathEntity* Pathfinder::createEntityPathTo(Entity* var1, double var2, double var4, double var6, float var8)
{
	this->path->clearPath();
	this->pointMap->clearMap();
	PathPoint* var9 = this->openPoint(MathHelper::floor_double(var1->boundingBox->minX), MathHelper::floor_double(var1->boundingBox->minY), MathHelper::floor_double(var1->boundingBox->minZ));
	PathPoint* var10 = this->openPoint(MathHelper::floor_double(var2 - (double)(var1->width / 2.0F)), MathHelper::floor_double(var4), MathHelper::floor_double(var6 - (double)(var1->width / 2.0F)));
	PathPoint* var11 = new PathPoint(MathHelper::floor_float(var1->width + 1.0F), MathHelper::floor_float(var1->height + 1.0F), MathHelper::floor_float(var1->width + 1.0F));
	PathEntity* var12 = this->addToPath(var1, var9, var10, var11, var8);
	return var12;
}

PathEntity* Pathfinder::addToPath(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5)
{
	var2->totalPathDistance = 0.0F;
	var2->distanceToNext = var2->distanceTo(var3);
	var2->distanceToTarget = var2->distanceToNext;
	this->path->clearPath();
	this->path->addPoint(var2);
	PathPoint* var6 = var2;
	while(!this->path->isPathEmpty()) {
	PathPoint* var7 = this->path->dequeue();
	if(var7->hash == var3->hash) {
	return this->createEntityPath(var2, var3);
	}
	if(var7->distanceTo(var3) < var6->distanceTo(var3)) {
	var6 = var7;
	}
	var7->isFirst = true;
	int var8 = this->findPathOptions(var1, var7, var4, var3, var5);
	for(int var9 = 0; var9 < var8; ++var9) {
	PathPoint* var10 = this->pathOptions[var9];
	float var11 = var7->totalPathDistance + var7->distanceTo(var10);
	if(!var10->isAssigned() || var11 < var10->totalPathDistance) {
	var10->previous = var7;
	var10->totalPathDistance = var11;
	var10->distanceToNext = var10->distanceTo(var3);
	if(var10->isAssigned()) {
	this->path->changeDistance(var10, var10->totalPathDistance + var10->distanceToNext);
	} else {
	var10->distanceToTarget = var10->totalPathDistance + var10->distanceToNext;
	this->path->addPoint(var10);
	}
	}
	}
	}
	if(var6 == var2) {
	return NULL;
	} else {
	return this->createEntityPath(var2, var6);
	}
}

int Pathfinder::findPathOptions(Entity* var1, PathPoint* var2, PathPoint* var3, PathPoint* var4, float var5)
{
	int var6 = 0;
	unsigned char var7 = 0;
	if(this->getVerticalOffset(var1, var2->xCoord, var2->yCoord + 1, var2->zCoord, var3) > 0) {
	var7 = 1;
	}
	PathPoint* var8 = this->getSafePoint(var1, var2->xCoord, var2->yCoord, var2->zCoord + 1, var3, var7);
	PathPoint* var9 = this->getSafePoint(var1, var2->xCoord - 1, var2->yCoord, var2->zCoord, var3, var7);
	PathPoint* var10 = this->getSafePoint(var1, var2->xCoord + 1, var2->yCoord, var2->zCoord, var3, var7);
	PathPoint* var11 = this->getSafePoint(var1, var2->xCoord, var2->yCoord, var2->zCoord - 1, var3, var7);
	if(var8 != NULL && !var8->isFirst && var8->distanceTo(var4) < var5) {
	this->pathOptions[var6++] = var8;
	}
	if(var9 != NULL && !var9->isFirst && var9->distanceTo(var4) < var5) {
	this->pathOptions[var6++] = var9;
	}
	if(var10 != NULL && !var10->isFirst && var10->distanceTo(var4) < var5) {
	this->pathOptions[var6++] = var10;
	}
	if(var11 != NULL && !var11->isFirst && var11->distanceTo(var4) < var5) {
	this->pathOptions[var6++] = var11;
	}
	return var6;
}

PathPoint* Pathfinder::getSafePoint(Entity* var1, int var2, int var3, int var4, PathPoint* var5, int var6)
{
	PathPoint* var7 = NULL;
	if(this->getVerticalOffset(var1, var2, var3, var4, var5) > 0) {
	var7 = this->openPoint(var2, var3, var4);
	}
	if(var7 == NULL && this->getVerticalOffset(var1, var2, var3 + var6, var4, var5) > 0) {
	var7 = this->openPoint(var2, var3 + var6, var4);
	var3 += var6;
	}
	if(var7 != NULL) {
	int var8 = 0;
	for(bool var9 = false; var3 > 0; --var3) {
	int var10 = this->getVerticalOffset(var1, var2, var3 - 1, var4, var5);
	if(var10 <= 0) {
	break;
	}
	if(var10 < 0) {
	return NULL;
	}
	++var8;
	if(var8 >= 4) {
	return NULL;
	}
	}
	if(var3 > 0) {
	var7 = this->openPoint(var2, var3, var4);
	}
	}
	return var7;
}

PathPoint* Pathfinder::openPoint(int var1, int var2, int var3)
{
	int var4 = var1 | (var2 << 10) | (var3 << 20);
	PathPoint* var5 = (PathPoint*)this->pointMap->lookup(var4);
	if(var5 == NULL) {
	var5 = new PathPoint(var1, var2, var3);
	this->pointMap->addKey(var4, var5);
	}
	return var5;
}

int Pathfinder::getVerticalOffset(Entity* var1, int var2, int var3, int var4, PathPoint* var5)
{
	for(int var6 = var2; var6 < var2 + var5->xCoord; ++var6) {
	for(int var7 = var3; var7 < var3 + var5->yCoord; ++var7) {
	for(int var8 = var4; var8 < var4 + var5->zCoord; ++var8) {
	Material* var9 = this->worldMap->getBlockMaterial(var2, var3, var4);
	if(var9->blocksMovement()) {
	return 0;
	}
	if(var9 == Material::s_water || var9 == Material::s_lava) {
	return -1;
	}
	}
	}
	}
	return 1;
}

PathEntity* Pathfinder::createEntityPath(PathPoint* var1, PathPoint* var2)
{
	int var3 = 1;
	PathPoint* var4;
	for(var4 = var2; var4->previous != NULL; var4 = var4->previous) {
	++var3;
	}
	JArray<PathPoint* > var5 = JArray<PathPoint* >(var3);
	var4 = var2;
	--var3;
	for(var5[var3] = var2; var4->previous != NULL; var5[var3] = var4) {
	var4 = var4->previous;
	--var3;
	}
	return new PathEntity(var5);
}

void Pathfinder::initStatics()
{
}
