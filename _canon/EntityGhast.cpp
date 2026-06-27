#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityGhast.h"
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
#include "EntityFireball.h"
#include "EntityFish.h"
#include "EntityFlying.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IMobs.h"
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

EntityGhast::EntityGhast(World* var1)
	: EntityFlying(var1)
	, courseChangeCooldown(0)
	, waypointX(0)
	, waypointY(0)
	, waypointZ(0)
	, targetedEntity(NULL)
	, aggroCooldown(0)
	, prevAttackCounter(0)
	, attackCounter(0)
{
	this->courseChangeCooldown = 0;
	this->targetedEntity = NULL;
	this->aggroCooldown = 0;
	this->prevAttackCounter = 0;
	this->attackCounter = 0;
	this->texture = "/mob/ghast.png";
	this->setSize(4.0F, 4.0F);
	this->isImmuneToFire = true;
}

EntityGhast::~EntityGhast()
{
}

void EntityGhast::updateEntityActionState()
{
	if(this->worldObj->difficultySetting == 0) {
	this->setEntityDead();
	}
	this->prevAttackCounter = this->attackCounter;
	double var1 = this->waypointX - this->posX;
	double var3 = this->waypointY - this->posY;
	double var5 = this->waypointZ - this->posZ;
	double var7 = (double)MathHelper::sqrt_double(var1 * var1 + var3 * var3 + var5 * var5);
	if(var7 < 1.0 || var7 > 60.0) {
	this->waypointX = this->posX + (double)((this->rand->nextFloat() * 2.0F - 1.0F) * 16.0F);
	this->waypointY = this->posY + (double)((this->rand->nextFloat() * 2.0F - 1.0F) * 16.0F);
	this->waypointZ = this->posZ + (double)((this->rand->nextFloat() * 2.0F - 1.0F) * 16.0F);
	}
	if(this->courseChangeCooldown-- <= 0) {
	this->courseChangeCooldown += this->rand->nextInt(5) + 2;
	if(this->isCourseTraversable(this->waypointX, this->waypointY, this->waypointZ, var7)) {
	this->motionX += var1 / var7 * 0.1;
	this->motionY += var3 / var7 * 0.1;
	this->motionZ += var5 / var7 * 0.1;
	} else {
	this->waypointX = this->posX;
	this->waypointY = this->posY;
	this->waypointZ = this->posZ;
	}
	}
	if(this->targetedEntity != NULL && this->targetedEntity->isDead) {
	this->targetedEntity = NULL;
	}
	if(this->targetedEntity == NULL || this->aggroCooldown-- <= 0) {
	this->targetedEntity = this->worldObj->getClosestPlayerToEntity(this, 100.0);
	if(this->targetedEntity != NULL) {
	this->aggroCooldown = 20;
	}
	}
	double var9 = 64.0;
	if(this->targetedEntity != NULL && this->targetedEntity->getDistanceSqToEntity(this) < var9 * var9) {
	double var11 = this->targetedEntity->posX - this->posX;
	double var13 = this->targetedEntity->boundingBox->minY + (double)(this->targetedEntity->height / 2.0F) - (this->posY + (double)(this->height / 2.0F));
	double var15 = this->targetedEntity->posZ - this->posZ;
	this->renderYawOffset = this->rotationYaw = -((float)atan2f(var11, var15)) * 180.0F / (float)3.1415927F;
	if(this->canEntityBeSeen(this->targetedEntity)) {
	if(this->attackCounter == 10) {
	this->worldObj->playSoundAtEntity(this,"mob.ghast.charge",this->getSoundVolume(),(this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	}
	++this->attackCounter;
	if(this->attackCounter == 20) {
	this->worldObj->playSoundAtEntity(this,"mob.ghast.fireball",this->getSoundVolume(),(this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	EntityFireball* var17 = new EntityFireball(this->worldObj, this, var11, var13, var15);
	double var18 = 4.0;
	Vec3D* var20 = this->getLook(1.0F);
	var17->posX = this->posX + var20->xCoord * var18;
	var17->posY = this->posY + (double)(this->height / 2.0F) + 0.5;
	var17->posZ = this->posZ + var20->zCoord * var18;
	this->worldObj->entityJoinedWorld(var17);
	this->attackCounter = -40;
	}
	} else if(this->attackCounter > 0) {
	--this->attackCounter;
	}
	} else {
	this->renderYawOffset = this->rotationYaw = -((float)atan2f(this->motionX, this->motionZ)) * 180.0F / (float)3.1415927F;
	if(this->attackCounter > 0) {
	--this->attackCounter;
	}
	}
	this->texture = this->attackCounter > 10 ? "/mob/ghast_fire.png" : "/mob/ghast.png";
}

bool EntityGhast::isCourseTraversable(double var1, double var3, double var5, double var7)
{
	double var9 = (this->waypointX - this->posX) / var7;
	double var11 = (this->waypointY - this->posY) / var7;
	double var13 = (this->waypointZ - this->posZ) / var7;
	AxisAlignedBB* var15 = this->boundingBox->copy();
	for(int var16 = 1; (double)var16 < var7; ++var16) {
	var15->offset(var9, var11, var13);
	if(this->worldObj->getCollidingBoundingBoxes(this, var15).size() > 0) {
	return false;
	}
	}
	return true;
}

JavaString EntityGhast::getLivingSound()
{
	return "mob.ghast.moan";
}

JavaString EntityGhast::getHurtSound()
{
	return "mob.ghast.scream";
}

JavaString EntityGhast::getDeathSound()
{
	return "mob.ghast.death";
}

int EntityGhast::getDropItemId()
{
	return Item::s_gunpowder->shiftedIndex;
}

float EntityGhast::getSoundVolume()
{
	return 10.0F;
}

bool EntityGhast::getCanSpawnHere()
{
	return this->rand->nextInt(20) == 0 && EntityFlying::getCanSpawnHere() && this->worldObj->difficultySetting > 0;
}

int EntityGhast::getMaxSpawnedInChunk()
{
	return 1;
}

void EntityGhast::initStatics()
{
}
