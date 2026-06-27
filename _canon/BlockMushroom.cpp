#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockMushroom.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockMushroom.h"
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

BlockMushroom::BlockMushroom(int var1, int var2)
	: BlockFlower(var1, var2)
{
	float var3 = 0.2F;
	this->setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 2.0F, 0.5F + var3);
}

BlockMushroom::~BlockMushroom()
{
}

bool BlockMushroom::canThisPlantGrowOnThisBlockID(int var1)
{
	return Block::s_opaqueCubeLookup[var1];
}

bool BlockMushroom::canBlockStay(World* var1, int var2, int var3, int var4)
{
	return var1->getBlockLightValue(var2, var3, var4) <= 13 && this->canThisPlantGrowOnThisBlockID(var1->getBlockId(var2, var3 - 1, var4));
}

void BlockMushroom::initStatics()
{
}
