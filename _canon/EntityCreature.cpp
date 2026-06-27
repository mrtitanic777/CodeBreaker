#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityCreature.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreature.h"
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
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
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
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

EntityCreature::EntityCreature(World* var1)
	: EntityLiving(var1)
	, pathToEntity(NULL)
	, playerToAttack(NULL)
	, hasAttacked(0)
{
	this->hasAttacked = false;
}

EntityCreature::~EntityCreature()
{
}

void EntityCreature::updateEntityActionState()
{
	this->hasAttacked = false;
	float var1 = 16.0F;
	if(this->playerToAttack == NULL) {
	this->playerToAttack = this->findPlayerToAttack();
	if(this->playerToAttack != NULL) {
	this->pathToEntity = this->worldObj->getPathToEntity(this, this->playerToAttack, var1);
	}
	} else if(!this->playerToAttack->isEntityAlive()) {
	this->playerToAttack = NULL;
	} else {
	float var2 = this->playerToAttack->getDistanceToEntity(this);
	if(this->canEntityBeSeen(this->playerToAttack)) {
	this->attackEntity(this->playerToAttack, var2);
	}
	}
	if(this->hasAttacked || this->playerToAttack == NULL || this->pathToEntity != NULL && this->rand->nextInt(20) != 0) {
	if(this->pathToEntity == NULL && this->rand->nextInt(80) == 0 || this->rand->nextInt(80) == 0) {
	bool var21 = false;
	int var3 = -1;
	int var4 = -1;
	int var5 = -1;
	float var6 = -99999.0F;
	for(int var7 = 0; var7 < 10; ++var7) {
	int var8 = MathHelper::floor_double(this->posX + (double)this->rand->nextInt(13) - 6.0);
	int var9 = MathHelper::floor_double(this->posY + (double)this->rand->nextInt(7) - 3.0);
	int var10 = MathHelper::floor_double(this->posZ + (double)this->rand->nextInt(13) - 6.0);
	float var11 = this->getBlockPathWeight(var8, var9, var10);
	if(var11 > var6) {
	var6 = var11;
	var3 = var8;
	var4 = var9;
	var5 = var10;
	var21 = true;
	}
	}
	if(var21) {
	this->pathToEntity = this->worldObj->getEntityPathToXYZ(this, var3, var4, var5, 10.0F);
	}
	}
	} else {
	this->pathToEntity = this->worldObj->getPathToEntity(this, this->playerToAttack, var1);
	}
	int var22 = MathHelper::floor_double(this->boundingBox->minY);
	bool var23 = this->handleWaterMovement();
	bool var24 = this->handleLavaMovement();
	this->rotationPitch = 0.0F;
	if(this->pathToEntity != NULL && this->rand->nextInt(100) != 0) {
	Vec3D* var25 = this->pathToEntity->getPosition(this);
	double var26 = (double)(this->width * 2.0F);
	while(var25 != NULL && var25->squareDistanceTo(this->posX, var25->yCoord, this->posZ) < var26 * var26) {
	this->pathToEntity->incrementPathIndex();
	if(this->pathToEntity->isFinished()) {
	var25 = NULL;
	this->pathToEntity = NULL;
	} else {
	var25 = this->pathToEntity->getPosition(this);
	}
	}
	this->isJumping = false;
	if(var25 != NULL) {
	double var27 = var25->xCoord - this->posX;
	double var28 = var25->zCoord - this->posZ;
	double var12 = var25->yCoord - (double)var22;
	float var14 = (float)(atan2f(var28, var27) * 180.0 / (double)((float)3.1415927F)) - 90.0F;
	float var15 = var14 - this->rotationYaw;
	for(this->moveForward = this->moveSpeed; var15 < -180.0F; var15 += 360.0F) {
	}
	while(var15 >= 180.0F) {
	var15 -= 360.0F;
	}
	if(var15 > 30.0F) {
	var15 = 30.0F;
	}
	if(var15 < -30.0F) {
	var15 = -30.0F;
	}
	this->rotationYaw += var15;
	if(this->hasAttacked && this->playerToAttack != NULL) {
	double var16 = this->playerToAttack->posX - this->posX;
	double var18 = this->playerToAttack->posZ - this->posZ;
	float var20 = this->rotationYaw;
	this->rotationYaw = (float)(atan2f(var18, var16) * 180.0 / (double)((float)3.1415927F)) - 90.0F;
	var15 = (var20 - this->rotationYaw + 90.0F) * (float)3.1415927F / 180.0F;
	this->moveStrafing = -MathHelper::sin(var15) * this->moveForward * 1.0F;
	this->moveForward = MathHelper::cos(var15) * this->moveForward * 1.0F;
	}
	if(var12 > 0.0) {
	this->isJumping = true;
	}
	}
	if(this->playerToAttack != NULL) {
	this->faceEntity(this->playerToAttack, 30.0F);
	}
	if(this->isCollidedHorizontally) {
	this->isJumping = true;
	}
	if(this->rand->nextFloat() < 0.8F && (var23 || var24)) {
	this->isJumping = true;
	}
	} else {
	EntityLiving::updateEntityActionState();
	this->pathToEntity = NULL;
	}
}

void EntityCreature::attackEntity(Entity* var1, float var2)
{
}

float EntityCreature::getBlockPathWeight(int var1, int var2, int var3)
{
	return 0.0F;
}

Entity* EntityCreature::findPlayerToAttack()
{
	return NULL;
}

bool EntityCreature::getCanSpawnHere()
{
	int var1 = MathHelper::floor_double(this->posX);
	int var2 = MathHelper::floor_double(this->boundingBox->minY);
	int var3 = MathHelper::floor_double(this->posZ);
	return EntityLiving::getCanSpawnHere() && this->getBlockPathWeight(var1, var2, var3) >= 0.0F;
}

void EntityCreature::initStatics()
{
}
