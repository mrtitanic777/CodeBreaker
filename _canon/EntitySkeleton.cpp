#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntitySkeleton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySkeleton.h"
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
#include "EntityArrow.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
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

ItemStack* EntitySkeleton::s_defaultHeldItem;

EntitySkeleton::EntitySkeleton(World* var1)
	: EntityMobs(var1)
{
	this->texture = "/mob/skeleton.png";
}

EntitySkeleton::~EntitySkeleton()
{
}

JavaString EntitySkeleton::getLivingSound()
{
	return "mob.skeleton";
}

JavaString EntitySkeleton::getHurtSound()
{
	return "mob.skeletonhurt";
}

JavaString EntitySkeleton::getDeathSound()
{
	return "mob.skeletonhurt";
}

void EntitySkeleton::onLivingUpdate()
{
	if(this->worldObj->isDaytime()) {
	float var1 = this->getEntityBrightness(1.0F);
	if(var1 > 0.5F && this->worldObj->canBlockSeeTheSky(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY), MathHelper::floor_double(this->posZ)) && this->rand->nextFloat() * 30.0F < (var1 - 0.4F) * 2.0F) {
	this->fire = 300;
	}
	}
	EntityMobs::onLivingUpdate();
}

void EntitySkeleton::attackEntity(Entity* var1, float var2)
{
	if(var2 < 10.0F) {
	double var3 = var1->posX - this->posX;
	double var5 = var1->posZ - this->posZ;
	if(this->attackTime == 0) {
	EntityArrow* var7 = new EntityArrow(this->worldObj, this);
	var7->posY += (double)1.4F;
	double var8 = var1->posY - (double)0.2F - var7->posY;
	float var10 = MathHelper::sqrt_double(var3 * var3 + var5 * var5) * 0.2F;
	this->worldObj->playSoundAtEntity(this,"random.bow",1.0F,1.0F / (this->rand->nextFloat() * 0.4F + 0.8F));
	this->worldObj->entityJoinedWorld(var7);
	var7->setArrowHeading(var3, var8 + (double)var10, var5, 0.6F, 12.0F);
	this->attackTime = 30;
	}
	this->rotationYaw = (float)(atan2f(var5, var3) * 180.0 / (double)((float)3.1415927F)) - 90.0F;
	this->hasAttacked = true;
	}
}

void EntitySkeleton::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityMobs::writeEntityToNBT(var1);
}

void EntitySkeleton::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityMobs::readEntityFromNBT(var1);
}

int EntitySkeleton::getDropItemId()
{
	return Item::s_arrow->shiftedIndex;
}

ItemStack* EntitySkeleton::getHeldItem()
{
	return s_defaultHeldItem;
}

void EntitySkeleton::initStatics()
{
	EntitySkeleton::s_defaultHeldItem = new ItemStack(Item::s_bow, 1);
}
