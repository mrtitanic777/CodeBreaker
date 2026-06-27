#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockContainer.h"
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

BlockContainer::BlockContainer(int var1, Material* var2)
	: Block(var1, var2)
{
	s_isBlockContainer[var1] = true;
}

BlockContainer::BlockContainer(int var1, int var2, Material* var3)
	: Block(var1, var2, var3)
{
}

BlockContainer::~BlockContainer()
{
}

void BlockContainer::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	Block::onBlockAdded(var1, var2, var3, var4);
	var1->setBlockTileEntity(var2, var3, var4, this->SetBlockEntity());
}

void BlockContainer::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	Block::onBlockRemoval(var1, var2, var3, var4);
	var1->removeBlockTileEntity(var2, var3, var4);
}

void BlockContainer::initStatics()
{
}
