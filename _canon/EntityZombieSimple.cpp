#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from EntityZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityZombieSimple.h"
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

EntityZombieSimple::EntityZombieSimple(World* var1)
	: EntityMobs(var1)
{
	this->texture = "/mob/zombie.png";
	this->moveSpeed = 0.5F;
	this->attackStrength = 50;
	this->health *= 10;
	this->yOffset *= 6.0F;
	this->setSize(this->width * 6.0F, this->height * 6.0F);
}

EntityZombieSimple::~EntityZombieSimple()
{
}

float EntityZombieSimple::getBlockPathWeight(int var1, int var2, int var3)
{
	return this->worldObj->getLightBrightness(var1, var2, var3) - 0.5F;
}

void EntityZombieSimple::initStatics()
{
}
