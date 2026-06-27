#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemSpade.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSpade.h"
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

static Block* _ItemSpade_s_blocksEffectiveAgainst_data[] = { Block::s_grass, Block::s_dirt, Block::s_sand, Block::s_gravel, Block::s_snow, Block::s_blockSnow, Block::s_blockClay };
JArray<Block* > ItemSpade::s_blocksEffectiveAgainst(_ItemSpade_s_blocksEffectiveAgainst_data, 7);

ItemSpade::ItemSpade(int var1, int var2)
	: ItemTool(var1, 1, var2, s_blocksEffectiveAgainst)
{
}

ItemSpade::~ItemSpade()
{
}

bool ItemSpade::canHarvestBlock(Block* var1)
{
	return var1 == Block::s_snow ? true : var1 == Block::s_blockSnow;
}

void ItemSpade::initStatics()
{
}
