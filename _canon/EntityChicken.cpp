#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityChicken.h"
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
#include "EntityAnimals.h"
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

EntityChicken::EntityChicken(World* var1)
	: EntityAnimals(var1)
	, field_753_a(0)
	, wingRotation(0)
	, destPos(0)
	, prevDestPos(0)
	, prevWingRotation(0)
	, wingRotDelta(0)
	, timeUntilNextEgg(0)
{
	this->field_753_a = false;
	this->wingRotation = 0.0F;
	this->destPos = 0.0F;
	this->wingRotDelta = 1.0F;
	this->texture = "/mob/chicken.png";
	this->setSize(0.3F, 0.4F);
	this->health = 4;
	this->timeUntilNextEgg = this->rand->nextInt(6000) + 6000;
}

EntityChicken::~EntityChicken()
{
}

void EntityChicken::onLivingUpdate()
{
	EntityAnimals::onLivingUpdate();
	this->prevWingRotation = this->wingRotation;
	this->prevDestPos = this->destPos;
	this->destPos = (float)((double)this->destPos + (double)(this->onGround ? -1 : 4) * 0.3);
	if(this->destPos < 0.0F) {
	this->destPos = 0.0F;
	}
	if(this->destPos > 1.0F) {
	this->destPos = 1.0F;
	}
	if(!this->onGround && this->wingRotDelta < 1.0F) {
	this->wingRotDelta = 1.0F;
	}
	this->wingRotDelta = (float)((double)this->wingRotDelta * 0.9);
	if(!this->onGround && this->motionY < 0.0) {
	this->motionY *= 0.6;
	}
	this->wingRotation += this->wingRotDelta * 2.0F;
	if(!this->worldObj->multiplayerWorld && --this->timeUntilNextEgg <= 0) {
	this->worldObj->playSoundAtEntity(this,"mob.chickenplop",1.0F,(this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F);
	this->dropItem(Item::s_egg->shiftedIndex, 1);
	this->timeUntilNextEgg = this->rand->nextInt(6000) + 6000;
	}
}

void EntityChicken::fall(float var1)
{
}

void EntityChicken::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityAnimals::writeEntityToNBT(var1);
}

void EntityChicken::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityAnimals::readEntityFromNBT(var1);
}

JavaString EntityChicken::getLivingSound()
{
	return "mob.chicken";
}

JavaString EntityChicken::getHurtSound()
{
	return "mob.chickenhurt";
}

JavaString EntityChicken::getDeathSound()
{
	return "mob.chickenhurt";
}

int EntityChicken::getDropItemId()
{
	return Item::s_feather->shiftedIndex;
}

void EntityChicken::initStatics()
{
}
