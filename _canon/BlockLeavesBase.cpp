#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockLeavesBase.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeavesBase.h"
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
#include "IBlockAccess.h"
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

BlockLeavesBase::BlockLeavesBase(int var1, int var2, Material* var3, bool var4)
	: Block(var1, var2, var3)
	, graphicsLevel(0)
{
	this->graphicsLevel = var4;
}

BlockLeavesBase::~BlockLeavesBase()
{
}

bool BlockLeavesBase::isOpaqueCube()
{
	return false;
}

bool BlockLeavesBase::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	int var6 = var1->getBlockId(var2, var3, var4);
	return !this->graphicsLevel && var6 == this->blockID ? false : Block::shouldSideBeRendered(var1, var2, var3, var4, var5);
}

void BlockLeavesBase::initStatics()
{
}
