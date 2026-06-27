#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntitySlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySlime.h"
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

EntitySlime::EntitySlime(World* var1)
	: EntityLiving(var1)
	, squishFactor(0)
	, prevSquishFactor(0)
	, slimeJumpDelay(0)
	, slimeSize(0)
{
	this->slimeJumpDelay = 0;
	this->slimeSize = 1;
	this->texture = "/mob/slime.png";
	this->slimeSize = 1 << this->rand->nextInt(3);
	this->yOffset = 0.0F;
	this->slimeJumpDelay = this->rand->nextInt(20) + 10;
	this->setSlimeSize(this->slimeSize);
}

EntitySlime::~EntitySlime()
{
}

void EntitySlime::setSlimeSize(int var1)
{
	this->slimeSize = var1;
	this->setSize(0.6F * (float)var1, 0.6F * (float)var1);
	this->health = var1 * var1;
	this->setPosition(this->posX, this->posY, this->posZ);
}

void EntitySlime::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityLiving::writeEntityToNBT(var1);
	var1->setInteger("Size", this->slimeSize - 1);
}

void EntitySlime::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityLiving::readEntityFromNBT(var1);
	this->slimeSize = var1->getInteger("Size") + 1;
}

void EntitySlime::onUpdate()
{
	this->prevSquishFactor = this->squishFactor;
	bool var1 = this->onGround;
	EntityLiving::onUpdate();
	if(this->onGround && !var1) {
	for(int var2 = 0; var2 < this->slimeSize * 8; ++var2) {
	float var3 = this->rand->nextFloat() * (float)3.1415927F * 2.0F;
	float var4 = this->rand->nextFloat() * 0.5F + 0.5F;
	float var5 = MathHelper::sin(var3) * (float)this->slimeSize * 0.5F * var4;
	float var6 = MathHelper::cos(var3) * (float)this->slimeSize * 0.5F * var4;
	this->worldObj->spawnParticle("slime",this->posX + (double)var5,this->boundingBox->minY,this->posZ + (double)var6,0.0,0.0,0.0);
	}
	if(this->slimeSize > 2) {
	this->worldObj->playSoundAtEntity(this,"mob.slime",this->getSoundVolume(),((this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F) / 0.8F);
	}
	this->squishFactor = -0.5F;
	}
	this->squishFactor *= 0.6F;
}

void EntitySlime::updateEntityActionState()
{
	EntityPlayer* var1 = this->worldObj->getClosestPlayerToEntity(this, 16.0);
	if(var1 != NULL) {
	this->faceEntity(var1, 10.0F);
	}
	if(this->onGround && this->slimeJumpDelay-- <= 0) {
	this->slimeJumpDelay = this->rand->nextInt(20) + 10;
	if(var1 != NULL) {
	this->slimeJumpDelay /= 3;
	}
	this->isJumping = true;
	if(this->slimeSize > 1) {
	this->worldObj->playSoundAtEntity(this,"mob.slime",this->getSoundVolume(),((this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F) * 0.8F);
	}
	this->squishFactor = 1.0F;
	this->moveStrafing = 1.0F - this->rand->nextFloat() * 2.0F;
	this->moveForward = (float)(1 * this->slimeSize);
	} else {
	this->isJumping = false;
	if(this->onGround) {
	this->moveStrafing = this->moveForward = 0.0F;
	}
	}
}

void EntitySlime::setEntityDead()
{
	if(this->slimeSize > 1 && this->health == 0) {
	for(int var1 = 0; var1 < 4; ++var1) {
	float var2 = ((float)(var1 % 2) - 0.5F) * (float)this->slimeSize / 4.0F;
	float var3 = ((float)(var1 / 2) - 0.5F) * (float)this->slimeSize / 4.0F;
	EntitySlime* var4 = new EntitySlime(this->worldObj);
	var4->setSlimeSize(this->slimeSize / 2);
	var4->setLocationAndAngles(this->posX + (double)var2, this->posY + 0.5, this->posZ + (double)var3, this->rand->nextFloat() * 360.0F, 0.0F);
	this->worldObj->entityJoinedWorld(var4);
	}
	}
	EntityLiving::setEntityDead();
}

void EntitySlime::onCollideWithPlayer(EntityPlayer* var1)
{
	if(this->slimeSize > 1 && this->canEntityBeSeen(var1) && (double)this->getDistanceToEntity(var1) < 0.6 * (double)this->slimeSize && var1->canAttackEntity(this, this->slimeSize)) {
	this->worldObj->playSoundAtEntity(this,"mob.slimeattack",1.0F,(this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	}
}

JavaString EntitySlime::getHurtSound()
{
	return "mob.slime";
}

JavaString EntitySlime::getDeathSound()
{
	return "mob.slime";
}

int EntitySlime::getDropItemId()
{
	return this->slimeSize == 1 ? Item::s_slimeBall->shiftedIndex : 0;
}

bool EntitySlime::getCanSpawnHere()
{
	Chunk* var1 = this->worldObj->getChunkFromBlockCoords(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posZ));
	return (this->slimeSize == 1 || this->worldObj->difficultySetting > 0) && this->rand->nextInt(10) == 0 && var1->getRandomWithSeed(987234911LL)->nextInt(10) == 0 && this->posY < 16.0;
}

float EntitySlime::getSoundVolume()
{
	return 0.6F;
}

void EntitySlime::initStatics()
{
}
