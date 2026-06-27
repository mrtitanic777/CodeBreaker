#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockStep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockStep.h"
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

BlockStep::BlockStep(int var1, bool var2)
	: Block(var1, 6, Material::s_rock)
	, isDoubleSlab(0)
{
	this->isDoubleSlab = var2;
	if(!var2) {
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 0.5F, 1.0F);
	}
	this->setLightOpacity(255);
}

BlockStep::~BlockStep()
{
}

int BlockStep::getBlockTextureFromSide(int var1)
{
	return var1 <= 1 ? 6 : 5;
}

bool BlockStep::isOpaqueCube()
{
	return this->isDoubleSlab;
}

void BlockStep::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	if(this == Block::s_stairSingle) {
	}
}

void BlockStep::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	if(this != Block::s_stairSingle) {
	Block::onBlockAdded(var1, var2, var3, var4);
	}
	int var5 = var1->getBlockId(var2, var3 - 1, var4);
	if(var5 == s_stairSingle->blockID) {
	var1->setBlockWithNotify(var2, var3, var4, 0);
	var1->setBlockWithNotify(var2, var3 - 1, var4, Block::s_stairDouble->blockID);
	}
}

int BlockStep::idDropped(int var1, Random* var2)
{
	return Block::s_stairSingle->blockID;
}

bool BlockStep::renderAsNormalBlock()
{
	return this->isDoubleSlab;
}

bool BlockStep::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	if(this != Block::s_stairSingle) {
	Block::shouldSideBeRendered(var1, var2, var3, var4, var5);
	}
	return var5 == 1 ? true : (!Block::shouldSideBeRendered(var1, var2, var3, var4, var5) ? false : (var5 == 0 ? true : var1->getBlockId(var2, var3, var4) != this->blockID));
}

void BlockStep::initStatics()
{
}
