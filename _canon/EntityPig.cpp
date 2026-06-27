#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPig.h"
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

EntityPig::EntityPig(World* var1)
	: EntityAnimals(var1)
	, rideable(0)
{
	this->rideable = false;
	this->texture = "/mob/pig.png";
	this->setSize(0.9F, 0.9F);
	this->rideable = false;
}

EntityPig::~EntityPig()
{
}

void EntityPig::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityAnimals::writeEntityToNBT(var1);
	var1->setBoolean("Saddle", this->rideable);
}

void EntityPig::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityAnimals::readEntityFromNBT(var1);
	this->rideable = var1->getBoolean("Saddle");
}

JavaString EntityPig::getLivingSound()
{
	return "mob.pig";
}

JavaString EntityPig::getHurtSound()
{
	return "mob.pig";
}

JavaString EntityPig::getDeathSound()
{
	return "mob.pigdeath";
}

bool EntityPig::interact(EntityPlayer* var1)
{
	if(this->rideable) {
	var1->mountEntity(this);
	return true;
	} else {
	return false;
	}
}

int EntityPig::getDropItemId()
{
	return Item::s_porkRaw->shiftedIndex;
}

void EntityPig::initStatics()
{
}
