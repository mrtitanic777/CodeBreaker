#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntitySpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySpider.h"
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

EntitySpider::EntitySpider(World* var1)
	: EntityMobs(var1)
{
	this->texture = "/mob/spider.png";
	this->setSize(1.4F, 0.9F);
	this->moveSpeed = 0.8F;
}

EntitySpider::~EntitySpider()
{
}

double EntitySpider::getMountedYOffset()
{
	return (double)this->height * 0.75 - 0.5;
}

Entity* EntitySpider::findPlayerToAttack()
{
	float var1 = this->getEntityBrightness(1.0F);
	if(var1 < 0.5F) {
	double var2 = 16.0;
	return this->worldObj->getClosestPlayerToEntity(this, var2);
	} else {
	return NULL;
	}
}

JavaString EntitySpider::getLivingSound()
{
	return "mob.spider";
}

JavaString EntitySpider::getHurtSound()
{
	return "mob.spider";
}

JavaString EntitySpider::getDeathSound()
{
	return "mob.spiderdeath";
}

void EntitySpider::attackEntity(Entity* var1, float var2)
{
	float var3 = this->getEntityBrightness(1.0F);
	if(var3 > 0.5F && this->rand->nextInt(100) == 0) {
	this->playerToAttack = NULL;
	} else {
	if(var2 > 2.0F && var2 < 6.0F && this->rand->nextInt(10) == 0) {
	if(this->onGround) {
	double var4 = var1->posX - this->posX;
	double var6 = var1->posZ - this->posZ;
	float var8 = MathHelper::sqrt_double(var4 * var4 + var6 * var6);
	this->motionX = var4 / (double)var8 * 0.5 * (double)0.8F + this->motionX * (double)0.2F;
	this->motionZ = var6 / (double)var8 * 0.5 * (double)0.8F + this->motionZ * (double)0.2F;
	this->motionY = (double)0.4F;
	}
	} else {
	EntityMobs::attackEntity(var1, var2);
	}
	}
}

void EntitySpider::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityMobs::writeEntityToNBT(var1);
}

void EntitySpider::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityMobs::readEntityFromNBT(var1);
}

int EntitySpider::getDropItemId()
{
	return Item::s_silk->shiftedIndex;
}

void EntitySpider::initStatics()
{
}
