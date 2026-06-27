#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from AxisAlignedBB.java
// Target: Xbox 360 / C++03 / DirectX 9

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

JList<void*> AxisAlignedBB::s_boundingBoxes;
int AxisAlignedBB::s_numBoundingBoxesInUse = 0;

AxisAlignedBB::AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11)
	: minX(0)
	, minY(0)
	, minZ(0)
	, maxX(0)
	, maxY(0)
	, maxZ(0)
{
	this->minX = var1;
	this->minY = var3;
	this->minZ = var5;
	this->maxX = var7;
	this->maxY = var9;
	this->maxZ = var11;
}

AxisAlignedBB::~AxisAlignedBB()
{
}

AxisAlignedBB* AxisAlignedBB::getBoundingBox(double var0, double var2, double var4, double var6, double var8, double var10)
{
	return new AxisAlignedBB(var0, var2, var4, var6, var8, var10);
}

void AxisAlignedBB::clearBoundingBoxPool()
{
	s_numBoundingBoxesInUse = 0;
}

AxisAlignedBB* AxisAlignedBB::getBoundingBoxFromPool(double var0, double var2, double var4, double var6, double var8, double var10)
{
	if(s_numBoundingBoxesInUse >= s_boundingBoxes.size()) {
	s_boundingBoxes.push_back(getBoundingBox(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	}
	return ((AxisAlignedBB*)s_boundingBoxes[s_numBoundingBoxesInUse++])->setBounds(var0, var2, var4, var6, var8, var10);
}

AxisAlignedBB* AxisAlignedBB::setBounds(double var1, double var3, double var5, double var7, double var9, double var11)
{
	this->minX = var1;
	this->minY = var3;
	this->minZ = var5;
	this->maxX = var7;
	this->maxY = var9;
	this->maxZ = var11;
	return this;
}

AxisAlignedBB* AxisAlignedBB::addCoord(double var1, double var3, double var5)
{
	double var7 = this->minX;
	double var9 = this->minY;
	double var11 = this->minZ;
	double var13 = this->maxX;
	double var15 = this->maxY;
	double var17 = this->maxZ;
	if(var1 < 0.0) {
	var7 += var1;
	}
	if(var1 > 0.0) {
	var13 += var1;
	}
	if(var3 < 0.0) {
	var9 += var3;
	}
	if(var3 > 0.0) {
	var15 += var3;
	}
	if(var5 < 0.0) {
	var11 += var5;
	}
	if(var5 > 0.0) {
	var17 += var5;
	}
	return getBoundingBoxFromPool(var7, var9, var11, var13, var15, var17);
}

AxisAlignedBB* AxisAlignedBB::expands(double var1, double var3, double var5)
{
	double var7 = this->minX - var1;
	double var9 = this->minY - var3;
	double var11 = this->minZ - var5;
	double var13 = this->maxX + var1;
	double var15 = this->maxY + var3;
	double var17 = this->maxZ + var5;
	return getBoundingBoxFromPool(var7, var9, var11, var13, var15, var17);
}

AxisAlignedBB* AxisAlignedBB::getOffsetBoundingBox(double var1, double var3, double var5)
{
	return getBoundingBoxFromPool(this->minX + var1, this->minY + var3, this->minZ + var5, this->maxX + var1, this->maxY + var3, this->maxZ + var5);
}

double AxisAlignedBB::calculateXOffset(AxisAlignedBB* var1, double var2)
{
	if(var1->maxY > this->minY && var1->minY < this->maxY) {
	if(var1->maxZ > this->minZ && var1->minZ < this->maxZ) {
	double var4;
	if(var2 > 0.0 && var1->maxX <= this->minX) {
	var4 = this->minX - var1->maxX;
	if(var4 < var2) {
	var2 = var4;
	}
	}
	if(var2 < 0.0 && var1->minX >= this->maxX) {
	var4 = this->maxX - var1->minX;
	if(var4 > var2) {
	var2 = var4;
	}
	}
	return var2;
	} else {
	return var2;
	}
	} else {
	return var2;
	}
}

double AxisAlignedBB::calculateYOffset(AxisAlignedBB* var1, double var2)
{
	if(var1->maxX > this->minX && var1->minX < this->maxX) {
	if(var1->maxZ > this->minZ && var1->minZ < this->maxZ) {
	double var4;
	if(var2 > 0.0 && var1->maxY <= this->minY) {
	var4 = this->minY - var1->maxY;
	if(var4 < var2) {
	var2 = var4;
	}
	}
	if(var2 < 0.0 && var1->minY >= this->maxY) {
	var4 = this->maxY - var1->minY;
	if(var4 > var2) {
	var2 = var4;
	}
	}
	return var2;
	} else {
	return var2;
	}
	} else {
	return var2;
	}
}

double AxisAlignedBB::calculateZOffset(AxisAlignedBB* var1, double var2)
{
	if(var1->maxX > this->minX && var1->minX < this->maxX) {
	if(var1->maxY > this->minY && var1->minY < this->maxY) {
	double var4;
	if(var2 > 0.0 && var1->maxZ <= this->minZ) {
	var4 = this->minZ - var1->maxZ;
	if(var4 < var2) {
	var2 = var4;
	}
	}
	if(var2 < 0.0 && var1->minZ >= this->maxZ) {
	var4 = this->maxZ - var1->minZ;
	if(var4 > var2) {
	var2 = var4;
	}
	}
	return var2;
	} else {
	return var2;
	}
	} else {
	return var2;
	}
}

bool AxisAlignedBB::intersectsWith(AxisAlignedBB* var1)
{
	return var1->maxX > this->minX && var1->minX < this->maxX ? (var1->maxY > this->minY && var1->minY < this->maxY ? var1->maxZ > this->minZ && var1->minZ < this->maxZ : false) : false;
}

AxisAlignedBB* AxisAlignedBB::offset(double var1, double var3, double var5)
{
	this->minX += var1;
	this->minY += var3;
	this->minZ += var5;
	this->maxX += var1;
	this->maxY += var3;
	this->maxZ += var5;
	return this;
}

bool AxisAlignedBB::isVecInside(Vec3D* var1)
{
	return var1->xCoord > this->minX && var1->xCoord < this->maxX ? (var1->yCoord > this->minY && var1->yCoord < this->maxY ? var1->zCoord > this->minZ && var1->zCoord < this->maxZ : false) : false;
}

double AxisAlignedBB::getAverageEdgeLength()
{
	double var1 = this->maxX - this->minX;
	double var3 = this->maxY - this->minY;
	double var5 = this->maxZ - this->minZ;
	return (var1 + var3 + var5) / 3.0;
}

AxisAlignedBB* AxisAlignedBB::copy()
{
	return getBoundingBoxFromPool(this->minX, this->minY, this->minZ, this->maxX, this->maxY, this->maxZ);
}

MovingObjectPosition* AxisAlignedBB::calculateIntercept(Vec3D* var1, Vec3D* var2)
{
	Vec3D* var3 = var1->getIntermediateWithXValue(var2, this->minX);
	Vec3D* var4 = var1->getIntermediateWithXValue(var2, this->maxX);
	Vec3D* var5 = var1->getIntermediateWithYValue(var2, this->minY);
	Vec3D* var6 = var1->getIntermediateWithYValue(var2, this->maxY);
	Vec3D* var7 = var1->getIntermediateWithZValue(var2, this->minZ);
	Vec3D* var8 = var1->getIntermediateWithZValue(var2, this->maxZ);
	if(!this->isVecInYZ(var3)) {
	var3 = NULL;
	}
	if(!this->isVecInYZ(var4)) {
	var4 = NULL;
	}
	if(!this->isVecInXZ(var5)) {
	var5 = NULL;
	}
	if(!this->isVecInXZ(var6)) {
	var6 = NULL;
	}
	if(!this->isVecInXY(var7)) {
	var7 = NULL;
	}
	if(!this->isVecInXY(var8)) {
	var8 = NULL;
	}
	Vec3D* var9 = NULL;
	if(var3 != NULL && (var9 == NULL || var1->squareDistanceTo(var3) < var1->squareDistanceTo(var9))) {
	var9 = var3;
	}
	if(var4 != NULL && (var9 == NULL || var1->squareDistanceTo(var4) < var1->squareDistanceTo(var9))) {
	var9 = var4;
	}
	if(var5 != NULL && (var9 == NULL || var1->squareDistanceTo(var5) < var1->squareDistanceTo(var9))) {
	var9 = var5;
	}
	if(var6 != NULL && (var9 == NULL || var1->squareDistanceTo(var6) < var1->squareDistanceTo(var9))) {
	var9 = var6;
	}
	if(var7 != NULL && (var9 == NULL || var1->squareDistanceTo(var7) < var1->squareDistanceTo(var9))) {
	var9 = var7;
	}
	if(var8 != NULL && (var9 == NULL || var1->squareDistanceTo(var8) < var1->squareDistanceTo(var9))) {
	var9 = var8;
	}
	if(var9 == NULL) {
	return NULL;
	} else {
	unsigned char var10 = -1;
	if(var9 == var3) {
	var10 = 4;
	}
	if(var9 == var4) {
	var10 = 5;
	}
	if(var9 == var5) {
	var10 = 0;
	}
	if(var9 == var6) {
	var10 = 1;
	}
	if(var9 == var7) {
	var10 = 2;
	}
	if(var9 == var8) {
	var10 = 3;
	}
	return new MovingObjectPosition(0, 0, 0, var10, var9);
	}
}

bool AxisAlignedBB::isVecInYZ(Vec3D* var1)
{
	return var1 == NULL ? false : var1->yCoord >= this->minY && var1->yCoord <= this->maxY && var1->zCoord >= this->minZ && var1->zCoord <= this->maxZ;
}

bool AxisAlignedBB::isVecInXZ(Vec3D* var1)
{
	return var1 == NULL ? false : var1->xCoord >= this->minX && var1->xCoord <= this->maxX && var1->zCoord >= this->minZ && var1->zCoord <= this->maxZ;
}

bool AxisAlignedBB::isVecInXY(Vec3D* var1)
{
	return var1 == NULL ? false : var1->xCoord >= this->minX && var1->xCoord <= this->maxX && var1->yCoord >= this->minY && var1->yCoord <= this->maxY;
}

void AxisAlignedBB::setBB(AxisAlignedBB* var1)
{
	this->minX = var1->minX;
	this->minY = var1->minY;
	this->minZ = var1->minZ;
	this->maxX = var1->maxX;
	this->maxY = var1->maxY;
	this->maxZ = var1->maxZ;
}

void AxisAlignedBB::initStatics()
{
}
