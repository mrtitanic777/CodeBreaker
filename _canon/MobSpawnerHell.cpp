#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MobSpawnerHell.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MobSpawnerHell.h"
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
#include "EntityGhast.h"
#include "EntityItem.h"
#include "EntityPigZombie.h"
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

MobSpawnerHell::MobSpawnerHell()
{
	this->biomeMonsters = (JArrayInit<Class* >(), Class(typeid(EntityGhast)), Class(typeid(EntityPigZombie)));
	this->biomeCreatures = JArray<Class* >(0);
}

MobSpawnerHell::~MobSpawnerHell()
{
}

void MobSpawnerHell::initStatics()
{
}
