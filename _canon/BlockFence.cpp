#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockFence.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFence.h"
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

BlockFence::BlockFence(int var1, int var2)
	: Block(var1, var2, Material::s_wood)
{
}

BlockFence::~BlockFence()
{
}

void BlockFence::getCollidingBoundingBoxes(World* var1, int var2, int var3, int var4, AxisAlignedBB* var5, JList<void*> var6)
{
	var6.push_back(AxisAlignedBB::getBoundingBoxFromPool((double)var2, (double)var3, (double)var4, (double)(var2 + 1), (double)var3 + 1.5, (double)(var4 + 1)));
}

bool BlockFence::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	return var1->getBlockId(var2, var3 - 1, var4) == this->blockID ? false : (!var1->getBlockMaterial(var2, var3 - 1, var4)->isSolid() ? false : Block::canPlaceBlockAt(var1, var2, var3, var4));
}

bool BlockFence::isOpaqueCube()
{
	return false;
}

bool BlockFence::renderAsNormalBlock()
{
	return false;
}

int BlockFence::getRenderType()
{
	return 11;
}

void BlockFence::initStatics()
{
}
