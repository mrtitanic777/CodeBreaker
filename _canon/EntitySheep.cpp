#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntitySheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySheep.h"
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
#include "EntityLiving.h"
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

EntitySheep::EntitySheep(World* var1)
	: EntityAnimals(var1)
	, sheared(0)
{
	this->sheared = false;
	this->texture = "/mob/sheep.png";
	this->setSize(0.9F, 1.3F);
}

EntitySheep::~EntitySheep()
{
}

bool EntitySheep::canAttackEntity(Entity* var1, int var2)
{
	if(!this->worldObj->multiplayerWorld && !this->sheared && (dynamic_cast<EntityLiving*>(var1) != NULL)) {
	this->sheared = true;
	int var3 = 1 + this->rand->nextInt(3);
	for(int var4 = 0; var4 < var3; ++var4) {
	EntityItem* var5 = this->dropItemWithOffset(Block::s_cloth->blockID, 1, 1.0F);
	var5->motionY += (double)(this->rand->nextFloat() * 0.05F);
	var5->motionX += (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.1F);
	var5->motionZ += (double)((this->rand->nextFloat() - this->rand->nextFloat()) * 0.1F);
	}
	}
	return EntityAnimals::canAttackEntity(var1, var2);
}

void EntitySheep::writeEntityToNBT(NBTTagCompound* var1)
{
	EntityAnimals::writeEntityToNBT(var1);
	var1->setBoolean("Sheared", this->sheared);
}

void EntitySheep::readEntityFromNBT(NBTTagCompound* var1)
{
	EntityAnimals::readEntityFromNBT(var1);
	this->sheared = var1->getBoolean("Sheared");
}

JavaString EntitySheep::getLivingSound()
{
	return "mob.sheep";
}

JavaString EntitySheep::getHurtSound()
{
	return "mob.sheep";
}

JavaString EntitySheep::getDeathSound()
{
	return "mob.sheep";
}

void EntitySheep::initStatics()
{
}
