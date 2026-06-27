#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityZombie.h"
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

EntityZombie::EntityZombie(World* var1)
	: EntityMobs(var1)
{
	this->texture = "/mob/zombie.png";
	this->moveSpeed = 0.5F;
	this->attackStrength = 5;
}

EntityZombie::~EntityZombie()
{
}

void EntityZombie::onLivingUpdate()
{
	if(this->worldObj->isDaytime()) {
	float var1 = this->getEntityBrightness(1.0F);
	if(var1 > 0.5F && this->worldObj->canBlockSeeTheSky(MathHelper::floor_double(this->posX), MathHelper::floor_double(this->posY), MathHelper::floor_double(this->posZ)) && this->rand->nextFloat() * 30.0F < (var1 - 0.4F) * 2.0F) {
	this->fire = 300;
	}
	}
	EntityMobs::onLivingUpdate();
}

JavaString EntityZombie::getLivingSound()
{
	return "mob.zombie";
}

JavaString EntityZombie::getHurtSound()
{
	return "mob.zombiehurt";
}

JavaString EntityZombie::getDeathSound()
{
	return "mob.zombiedeath";
}

int EntityZombie::getDropItemId()
{
	return Item::s_feather->shiftedIndex;
}

void EntityZombie::initStatics()
{
}
