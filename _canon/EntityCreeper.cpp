#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCreeper.h"
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
#include "EntityMobs.h"
#include "EntityPlayer.h"
#include "EntitySkeleton.h"
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

EntityCreeper::EntityCreeper(World* var1)
	: EntityMobs(var1)
	, timeSinceIgnited(0)
	, lastActiveTime(0)
	, fuseTime(0)
	, creeperState(0)
	, prevCreeperState(0)
{
	this->fuseTime = 30;
	this->creeperState = -1;
	this->prevCreeperState = -1;
	this->texture = "/mob/creeper.png";
}

EntityCreeper::~EntityCreeper()
{
}

void EntityCreeper::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityMobs::writeEntityToNBT(var1);
}

void EntityCreeper::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityMobs::readEntityFromNBT(var1);
}

void EntityCreeper::handleHealthUpdate(unsigned char var1)
{
	EntityMobs::handleHealthUpdate(var1);
	if(var1 == 4) {
	if(this->timeSinceIgnited == 0) {
	this->worldObj->playSoundAtEntity(this, "random.fuse", 1.0F, 0.5F);
	}
	this->creeperState = 1;
	}
	if(var1 == 5) {
	this->creeperState = -1;
	}
}

void EntityCreeper::onUpdate()
{
	this->lastActiveTime = this->timeSinceIgnited;
	if(this->worldObj->multiplayerWorld) {
	this->timeSinceIgnited += this->creeperState;
	if(this->timeSinceIgnited < 0) {
	this->timeSinceIgnited = 0;
	}
	if(this->timeSinceIgnited >= this->fuseTime) {
	this->timeSinceIgnited = this->fuseTime;
	}
	}
	EntityMobs::onUpdate();
}

void EntityCreeper::updateEntityActionState()
{
	if(this->prevCreeperState != this->creeperState) {
	this->prevCreeperState = this->creeperState;
	if(this->creeperState > 0) {
	this->worldObj->setEntityState(this, (unsigned char)4);
	} else {
	this->worldObj->setEntityState(this, (unsigned char)5);
	}
	}
	this->lastActiveTime = this->timeSinceIgnited;
	if(this->worldObj->multiplayerWorld) {
	EntityMobs::updateEntityActionState();
	} else {
	if(this->timeSinceIgnited > 0 && this->creeperState < 0) {
	--this->timeSinceIgnited;
	}
	if(this->creeperState >= 0) {
	this->creeperState = 2;
	}
	EntityMobs::updateEntityActionState();
	if(this->creeperState != 1) {
	this->creeperState = -1;
	}
	}
}

JavaString EntityCreeper::getHurtSound()
{
	return "mob.creeper";
}

JavaString EntityCreeper::getDeathSound()
{
	return "mob.creeperdeath";
}

void EntityCreeper::onDeath(Entity* var1)
{
	EntityMobs::onDeath(var1);
	if((dynamic_cast<EntitySkeleton*>(var1) != NULL)) {
	this->dropItem(Item::s_record13->shiftedIndex + this->rand->nextInt(2), 1);
	}
}

void EntityCreeper::attackEntity(Entity* var1, float var2)
{
	if(this->creeperState <= 0 && var2 < 3.0F || this->creeperState > 0 && var2 < 7.0F) {
	if(this->timeSinceIgnited == 0) {
	this->worldObj->playSoundAtEntity(this, "random.fuse", 1.0F, 0.5F);
	}
	this->creeperState = 1;
	++this->timeSinceIgnited;
	if(this->timeSinceIgnited == this->fuseTime) {
	this->worldObj->createExplosion(this, this->posX, this->posY, this->posZ, 3.0F);
	this->setEntityDead();
	}
	this->hasAttacked = true;
	}
}

float EntityCreeper::getCreeperFlashIntensity(float var1)
{
	return ((float)this->lastActiveTime + (float)(this->timeSinceIgnited - this->lastActiveTime) * var1) / (float)(this->fuseTime - 2);
}

int EntityCreeper::getDropItemId()
{
	return Item::s_gunpowder->shiftedIndex;
}

void EntityCreeper::initStatics()
{
}
