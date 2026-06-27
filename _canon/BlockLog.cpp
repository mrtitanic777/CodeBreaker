#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockLog.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLog.h"
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

BlockLog::BlockLog(int var1)
	: Block(var1, Material::s_wood)
{
	this->blockIndexInTexture = 20;
}

BlockLog::~BlockLog()
{
}

int BlockLog::quantityDropped(Random* var1)
{
	return 1;
}

int BlockLog::idDropped(int var1, Random* var2)
{
	return Block::s_wood->blockID;
}

int BlockLog::getBlockTextureFromSide(int var1)
{
	return var1 == 1 ? 21 : (var1 == 0 ? 21 : 20);
}

void BlockLog::initStatics()
{
}
