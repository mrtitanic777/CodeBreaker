#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityFallingSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFallingSand.h"
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

EntityFallingSand::EntityFallingSand(World* var1)
	: Entity(var1)
	, blockID(0)
	, fallTime(0)
{
	this->fallTime = 0;
}

EntityFallingSand::EntityFallingSand(World* var1, float var2, float var3, float var4, int var5)
	: Entity(var1)
	, blockID(0)
	, fallTime(0)
{
	this->fallTime = 0;
	this->blockID = var5;
	this->preventEntitySpawning = true;
	this->setSize(0.98F, 0.98F);
	this->yOffset = this->height / 2.0F;
	this->setPosition((double)var2, (double)var3, (double)var4);
	this->motionX = 0.0;
	this->motionY = 0.0;
	this->motionZ = 0.0;
	this->entityWalks = false;
	this->prevPosX = (double)var2;
	this->prevPosY = (double)var3;
	this->prevPosZ = (double)var4;
}

EntityFallingSand::~EntityFallingSand()
{
}

bool EntityFallingSand::canBeCollidedWith()
{
	return !this->isDead;
}

void EntityFallingSand::onUpdate()
{
	if(this->blockID == 0) {
	this->setEntityDead();
	} else {
	this->prevPosX = this->posX;
	this->prevPosY = this->posY;
	this->prevPosZ = this->posZ;
	++this->fallTime;
	this->motionY -= (double)0.04F;
	this->moveEntity(this->motionX, this->motionY, this->motionZ);
	this->motionX *= (double)0.98F;
	this->motionY *= (double)0.98F;
	this->motionZ *= (double)0.98F;
	int var1 = MathHelper::floor_double(this->posX);
	int var2 = MathHelper::floor_double(this->posY);
	int var3 = MathHelper::floor_double(this->posZ);
	if(this->worldObj->getBlockId(var1, var2, var3) == this->blockID) {
	this->worldObj->setBlockWithNotify(var1, var2, var3, 0);
	}
	if(this->onGround) {
	this->motionX *= (double)0.7F;
	this->motionZ *= (double)0.7F;
	this->motionY *= -0.5;
	this->setEntityDead();
	if(!this->worldObj->canBlockBePlacedAt(this->blockID, var1, var2, var3, true) || !this->worldObj->setBlockWithNotify(var1, var2, var3, this->blockID)) {
	this->dropItem(this->blockID, 1);
	}
	} else if(this->fallTime > 100) {
	this->dropItem(this->blockID, 1);
	this->setEntityDead();
	}
	}
}

void EntityFallingSand::writeEntityToNBT(NBTTagCompound* var1)
{
	var1->setByte("Tile", (unsigned char)this->blockID);
}

void EntityFallingSand::readEntityFromNBT(NBTTagCompound* var1)
{
	this->blockID = var1->getByte("Tile") & 255;
}

float EntityFallingSand::getYOffsetForRender()
{
	return 0.0F;
}

World* EntityFallingSand::getWorld()
{
	return this->worldObj;
}

void EntityFallingSand::initStatics()
{
}
