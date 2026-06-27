#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityTNTPrimed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityTNTPrimed.h"
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

EntityTNTPrimed::EntityTNTPrimed(World* var1)
	: Entity(var1)
	, fuse(0)
{
	this->fuse = 0;
	this->preventEntitySpawning = true;
	this->setSize(0.98F, 0.98F);
	this->yOffset = this->height / 2.0F;
}

EntityTNTPrimed::EntityTNTPrimed(World* var1, double var2, double var4, double var6)
	: Entity(var1)
	, fuse(0)
{
	// inlined this(var1) -- C++03 lacks ctor delegation
	this->fuse = 0;
	this->preventEntitySpawning = true;
	this->setSize(0.98F, 0.98F);
	this->yOffset = this->height / 2.0F;
	this->setPosition(var2, var4, var6);
	float var8 = (float)(javaMathRandom() * (double)((float)3.1415927F) * 2.0);
	this->motionX = (double)(-MathHelper::sin(var8 * (float)3.1415927F / 180.0F) * 0.02F);
	this->motionY = (double)0.2F;
	this->motionZ = (double)(-MathHelper::cos(var8 * (float)3.1415927F / 180.0F) * 0.02F);
	this->entityWalks = false;
	this->fuse = 80;
	this->prevPosX = var2;
	this->prevPosY = var4;
	this->prevPosZ = var6;
}

EntityTNTPrimed::~EntityTNTPrimed()
{
}

bool EntityTNTPrimed::canBeCollidedWith()
{
	return !this->isDead;
}

void EntityTNTPrimed::onUpdate()
{
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	this->motionY -= (double)0.04F;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.98F;
	this->motionY *= (double)0.98F;
	this->motionZ *= (double)0.98F;
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	this->motionY *= -0.5;
	}
	if(this->fuse-- <= 0) {
	this->setEntityDead();
	this->explode();
	} else {
	this->worldObj->spawnParticle("smoke",this->posX,this->posY + 0.5,this->posZ,0.0,0.0,0.0);
	}
}

void EntityTNTPrimed::explode()
{
	float var1 = 4.0F;
	this->worldObj->createExplosion((Entity*)NULL, this->posX, this->posY, this->posZ, var1);
}

void EntityTNTPrimed::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setByte("Fuse", (unsigned char)this->fuse);
}

void EntityTNTPrimed::readEntityFromNBT(NBTTagCompound* var1)
{
	this->fuse = var1->getByte("Fuse");
}

float EntityTNTPrimed::getYOffsetForRender()
{
	return 0.0F;
}

void EntityTNTPrimed::initStatics()
{
}
