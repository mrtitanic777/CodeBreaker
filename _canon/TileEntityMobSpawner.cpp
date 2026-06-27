#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntityMobSpawner.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityMobSpawner.h"
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
#include "EntityList.h"
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

TileEntityMobSpawner::TileEntityMobSpawner()
	: delay(0)
	, spinAngle(0)
	, prevSpinAngle(0)
{
	this->delay = -1;
	this->entityID = "Pig";
	this->prevSpinAngle = 0.0;
	this->delay = 20;
}

TileEntityMobSpawner::~TileEntityMobSpawner()
{
}

bool TileEntityMobSpawner::anyPlayerInRange()
{
	return this->worldObj->getClosestPlayer((double)this->xCoord + 0.5, (double)this->yCoord + 0.5, (double)this->zCoord + 0.5, 16.0) != NULL;
}

void TileEntityMobSpawner::updateEntity()
{
	this->prevSpinAngle = this->spinAngle;
	if(this->anyPlayerInRange()) {
	double var1 = (double)((float)this->xCoord + this->worldObj->rand->nextFloat());
	double var3 = (double)((float)this->yCoord + this->worldObj->rand->nextFloat());
	double var5 = (double)((float)this->zCoord + this->worldObj->rand->nextFloat());
	this->worldObj->spawnParticle("smoke", var1, var3, var5, 0.0, 0.0, 0.0);
	this->worldObj->spawnParticle("flame", var1, var3, var5, 0.0, 0.0, 0.0);
	for(this->spinAngle += (double)(1000.0F / ((float)this->delay + 200.0F)); this->spinAngle > 360.0; this->prevSpinAngle -= 360.0) {
	this->spinAngle -= 360.0;
	}
	if(this->delay == -1) {
	this->updateDelay();
	}
	if(this->delay > 0) {
	--this->delay;
	} else {
	unsigned char var7 = 4;
	for(int var8 = 0; var8 < var7; ++var8) {
	EntityLiving* var9 = (EntityLiving*)((EntityLiving*)EntityList::createEntityInWorld(this->entityID, this->worldObj));
	if(var9 == NULL) {
	return;
	}
	int var10 = this->worldObj->getEntitiesWithinAABB(Class(typeid(*(var9))), AxisAlignedBB::getBoundingBoxFromPool((double)this->xCoord, (double)this->yCoord, (double)this->zCoord, (double)(this->xCoord + 1), (double)(this->yCoord + 1), (double)(this->zCoord + 1))->expands(8.0, 4.0, 8.0)).size();
	if(var10 >= 6) {
	this->updateDelay();
	return;
	}
	if(var9 != NULL) {
	double var11 = (double)this->xCoord + (this->worldObj->rand->nextDouble() - this->worldObj->rand->nextDouble()) * 4.0;
	double var13 = (double)(this->yCoord + this->worldObj->rand->nextInt(3) - 1);
	double var15 = (double)this->zCoord + (this->worldObj->rand->nextDouble() - this->worldObj->rand->nextDouble()) * 4.0;
	var9->setLocationAndAngles(var11, var13, var15, this->worldObj->rand->nextFloat() * 360.0F, 0.0F);
	if(var9->getCanSpawnHere()) {
	this->worldObj->entityJoinedWorld(var9);
	for(int var17 = 0; var17 < 20; ++var17) {
	var1 = (double)this->xCoord + 0.5 + ((double)this->worldObj->rand->nextFloat() - 0.5) * 2.0;
	var3 = (double)this->yCoord + 0.5 + ((double)this->worldObj->rand->nextFloat() - 0.5) * 2.0;
	var5 = (double)this->zCoord + 0.5 + ((double)this->worldObj->rand->nextFloat() - 0.5) * 2.0;
	this->worldObj->spawnParticle("smoke", var1, var3, var5, 0.0, 0.0, 0.0);
	this->worldObj->spawnParticle("flame", var1, var3, var5, 0.0, 0.0, 0.0);
	}
	var9->spawnExplosionParticle();
	this->updateDelay();
	}
	}
	}
	TileEntity::updateEntity();
	}
	}
}

void TileEntityMobSpawner::updateDelay()
{
	this->delay = 200 + this->worldObj->rand->nextInt(600);
}

void TileEntityMobSpawner::readFromNBT(NBTTagCompound* var1)
{
	TileEntity::readFromNBT(var1);
	this->entityID = var1->getString("EntityId");
	this->delay = var1->getShort("Delay");
}

void TileEntityMobSpawner::writeToNBT(NBTTagCompound* var1)
{
	TileEntity::writeToNBT(var1);
	var1->setString("EntityId", this->entityID);
	var1->setShort("Delay", (short)this->delay);
}

void TileEntityMobSpawner::initStatics()
{
}
