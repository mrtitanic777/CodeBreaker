#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPigZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPigZombie.h"
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
#include "EntityZombie.h"
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

ItemStack* EntityPigZombie::s_defaultHeldItem;

EntityPigZombie::EntityPigZombie(World* var1)
	: EntityZombie(var1)
	, angerLevel(0)
	, randomSoundDelay(0)
{
	this->angerLevel = 0;
	this->randomSoundDelay = 0;
	this->texture = "/mob/pigzombie.png";
	this->moveSpeed = 0.5F;
	this->attackStrength = 5;
	this->isImmuneToFire = true;
}

EntityPigZombie::~EntityPigZombie()
{
}

void EntityPigZombie::onUpdate()
{
	this->moveSpeed = this->playerToAttack != NULL ? 0.95F : 0.5F;
	if(this->randomSoundDelay > 0 && --this->randomSoundDelay == 0) {
	this->worldObj->playSoundAtEntity(this,"mob.zombiepig.zpigangry",this->getSoundVolume() * 2.0F,((this->rand->nextFloat() - this->rand->nextFloat()) * 0.2F + 1.0F) * 1.8F);
	}
	EntityZombie::onUpdate();
}

bool EntityPigZombie::getCanSpawnHere()
{
	return this->worldObj->difficultySetting > 0 && this->worldObj->checkIfAABBIsClear(this->boundingBox) && this->worldObj->getCollidingBoundingBoxes(this, this->boundingBox).size() == 0 && !this->worldObj->getIsAnyLiquid(this->boundingBox);
}

void EntityPigZombie::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityZombie::writeEntityToNBT(var1);
	var1->setShort("Anger", (short)this->angerLevel);
}

void EntityPigZombie::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityZombie::readEntityFromNBT(var1);
	this->angerLevel = var1->getShort("Anger");
}

Entity* EntityPigZombie::findPlayerToAttack()
{
	return this->angerLevel == 0 ? NULL : EntityZombie::findPlayerToAttack();
}

void EntityPigZombie::onLivingUpdate()
{
	EntityZombie::onLivingUpdate();
}

bool EntityPigZombie::canAttackEntity(Entity* var1, int var2)
{
	if((dynamic_cast<EntityPlayer*>(var1) != NULL)) {
	JList<void*> var3 = this->worldObj->getEntitiesWithinAABBExcludingEntity(this, this->boundingBox->expands(32.0, 32.0, 32.0));
	for(int var4 = 0; var4 < var3.size(); ++var4) {
	Entity* var5 = (Entity*)var3[var4];
	if((dynamic_cast<EntityPigZombie*>(var5) != NULL)) {
	EntityPigZombie* var6 = (EntityPigZombie*)var5;
	var6->becomeAngryAt(var1);
	}
	}
	this->becomeAngryAt(var1);
	}
	return EntityZombie::canAttackEntity(var1, var2);
}

void EntityPigZombie::becomeAngryAt(Entity* var1)
{
	this->playerToAttack = var1;
	this->angerLevel = 400 + this->rand->nextInt(400);
	this->randomSoundDelay = this->rand->nextInt(40);
}

JavaString EntityPigZombie::getLivingSound()
{
	return "mob.zombiepig.zpig";
}

JavaString EntityPigZombie::getHurtSound()
{
	return "mob.zombiepig.zpighurt";
}

JavaString EntityPigZombie::getDeathSound()
{
	return "mob.zombiepig.zpigdeath";
}

int EntityPigZombie::getDropItemId()
{
	return Item::s_porkCooked->shiftedIndex;
}

ItemStack* EntityPigZombie::getHeldItem()
{
	return s_defaultHeldItem;
}

void EntityPigZombie::initStatics()
{
	EntityPigZombie::s_defaultHeldItem = new ItemStack(Item::s_swordGold, 1);
}
