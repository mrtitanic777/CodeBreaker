#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ItemPickaxe.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemPickaxe.h"
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

static Block* _ItemPickaxe_s_blocksEffectiveAgainst_data[] = { Block::s_cobblestone, Block::s_stairDouble, Block::s_stairSingle, Block::s_stone, Block::s_cobblestoneMossy, Block::s_oreIron, Block::s_blockSteel, Block::s_oreCoal, Block::s_blockGold, Block::s_oreGold, Block::s_oreDiamond, Block::s_blockDiamond, Block::s_blockIce, Block::s_bloodStone };
JArray<Block* > ItemPickaxe::s_blocksEffectiveAgainst(_ItemPickaxe_s_blocksEffectiveAgainst_data, 14);

ItemPickaxe::ItemPickaxe(int var1, int var2)
	: ItemTool(var1, 2, var2, s_blocksEffectiveAgainst)
	, toolMaterialLevel(0)
{
	this->toolMaterialLevel = var2;
}

ItemPickaxe::~ItemPickaxe()
{
}

bool ItemPickaxe::canHarvestBlock(Block* var1)
{
	return var1 == Block::s_obsidian ? this->toolMaterialLevel == 3 : (var1 != Block::s_blockDiamond && var1 != Block::s_oreDiamond ? (var1 != Block::s_blockGold && var1 != Block::s_oreGold ? (var1 != Block::s_blockSteel && var1 != Block::s_oreIron ? (var1 != Block::s_oreRedstone && var1 != Block::s_oreRedstoneGlowing ? (var1->blockMaterial == Material::s_rock ? true : var1->blockMaterial == Material::s_iron) : this->toolMaterialLevel >= 2) : this->toolMaterialLevel >= 1) : this->toolMaterialLevel >= 2) : this->toolMaterialLevel >= 2);
}

void ItemPickaxe::initStatics()
{
}
