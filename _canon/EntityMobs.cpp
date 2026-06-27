#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityMobs.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"
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
#include "EntityCreature.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumSkyBlock.h"
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

EntityMobs::EntityMobs(World* var1)
	: EntityCreature(var1)
	, attackStrength(0)
{
	this->attackStrength = 2;
	this->health = 20;
}

EntityMobs::~EntityMobs()
{
}

void EntityMobs::onLivingUpdate()
{
	float var1 = this->getEntityBrightness(1.0F);
	if(var1 > 0.5F) {
	this->entityAge += 2;
	}
	EntityCreature::onLivingUpdate();
}

void EntityMobs::onUpdate()
{
	EntityCreature::onUpdate();
	if(this->worldObj->difficultySetting == 0) {
	this->setEntityDead();
	}
}

Entity* EntityMobs::findPlayerToAttack()
{
	EntityPlayer* var1 = this->worldObj->getClosestPlayerToEntity(this, 16.0);
	return var1 != NULL && this->canEntityBeSeen(var1) ? var1 : NULL;
}

bool EntityMobs::canAttackEntity(Entity* var1, int var2)
{
	if(EntityCreature::canAttackEntity(var1, var2)) {
	if(this->riddenByEntity != var1 && this->ridingEntity != var1) {
	if(var1 != this) {
	this->playerToAttack = var1;
	}
	return true;
	} else {
	return true;
	}
	} else {
	return false;
	}
}

void EntityMobs::attackEntity(Entity* var1, float var2)
{
	if((double)var2 < 2.5 && var1->boundingBox->maxY > this->boundingBox->minY && var1->boundingBox->minY < this->boundingBox->maxY) {
	this->attackTime = 20;
	var1->canAttackEntity(this, this->attackStrength);
	}
}

float EntityMobs::getBlockPathWeight(int var1, int var2, int var3)
{
	return 0.5F - this->worldObj->getLightBrightness(var1, var2, var3);
}

void EntityMobs::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityCreature::writeEntityToNBT(var1);
}

void EntityMobs::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityCreature::readEntityFromNBT(var1);
}

bool EntityMobs::getCanSpawnHere()
{
	int var1 = MathHelper::floor_double(this->posX);
	int var2 = MathHelper::floor_double(this->boundingBox->minY);
	int var3 = MathHelper::floor_double(this->posZ);
	if(this->worldObj->getSavedLightValue(EnumSkyBlock::Sky, var1, var2, var3) > this->rand->nextInt(32)) {
	return false;
	} else {
	int var4 = this->worldObj->getBlockLightValue(var1, var2, var3);
	return var4 <= this->rand->nextInt(8) && EntityCreature::getCanSpawnHere();
	}
}

void EntityMobs::initStatics()
{
}
