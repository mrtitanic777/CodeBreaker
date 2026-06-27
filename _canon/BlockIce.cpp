#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockIce.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockIce.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockBreakable.h"
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
#include "EnumSkyBlock.h"
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

BlockIce::BlockIce(int var1, int var2)
	: BlockBreakable(var1, var2, Material::s_ice, false)
{
	this->slipperiness = 0.98F;
	this->setTickOnLoad(true);
}

BlockIce::~BlockIce()
{
}

int BlockIce::getRenderBlockPass()
{
	return 1;
}

bool BlockIce::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return BlockBreakable::shouldSideBeRendered(var1, var2, var3, var4, 1 - var5);
}

void BlockIce::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	Material* var5 = var1->getBlockMaterial(var2, var3 - 1, var4);
	if(var5->blocksMovement() || var5->getIsLiquid()) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_waterStill->blockID);
	}
}

int BlockIce::quantityDropped(Random* var1)
{
	return 0;
}

void BlockIce::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var1->getSavedLightValue(EnumSkyBlock::Block, var2, var3, var4) > 11 - Block::s_lightOpacity[this->blockID]) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, Block::s_waterMoving->blockID);
	}
}

void BlockIce::initStatics()
{
}
