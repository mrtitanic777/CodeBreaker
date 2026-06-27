#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCow.h"
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

EntityCow::EntityCow(World* var1)
	: EntityAnimals(var1)
	, unusedBoolean(0)
{
	this->unusedBoolean = false;
	this->texture = "/mob/cow.png";
	this->setSize(0.9F, 1.3F);
}

EntityCow::~EntityCow()
{
}

void EntityCow::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityAnimals::writeEntityToNBT(var1);
}

void EntityCow::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityAnimals::readEntityFromNBT(var1);
}

JavaString EntityCow::getLivingSound()
{
	return "mob.cow";
}

JavaString EntityCow::getHurtSound()
{
	return "mob.cowhurt";
}

JavaString EntityCow::getDeathSound()
{
	return "mob.cowhurt";
}

float EntityCow::getSoundVolume()
{
	return 0.4F;
}

int EntityCow::getDropItemId()
{
	return Item::s_leather->shiftedIndex;
}

bool EntityCow::interact(EntityPlayer* var1)
{
	ItemStack* var2 = var1->inventory->getCurrentItem();
	if(var2 != NULL && var2->itemID == Item::s_bucketEmpty->shiftedIndex) {
	var1->inventory->setInventorySlotContents(var1->inventory->currentItem, new ItemStack(Item::s_bucketMilk));
	return true;
	} else {
	return false;
	}
}

void EntityCow::initStatics()
{
}
