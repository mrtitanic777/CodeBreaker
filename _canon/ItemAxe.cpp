#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemAxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemAxe.h"
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
#include "ItemTool.h"
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

static Block* _ItemAxe_s_blocksEffectiveAgainst_data[] = { Block::s_planks, Block::s_bookShelf, Block::s_wood, Block::s_crate };
JArray<Block* > ItemAxe::s_blocksEffectiveAgainst(_ItemAxe_s_blocksEffectiveAgainst_data, 4);

ItemAxe::ItemAxe(int var1, int var2)
	: ItemTool(var1, 3, var2, s_blocksEffectiveAgainst)
{
}

ItemAxe::~ItemAxe()
{
}

void ItemAxe::initStatics()
{
}
