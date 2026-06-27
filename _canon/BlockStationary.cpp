#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockStationary.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockStationary.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFluids.h"
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

BlockStationary::BlockStationary(int var1, Material* var2)
	: BlockFluids(var1, var2)
{
	this->setTickOnLoad(false);
	if(var2 == Material::s_lava) {
	this->setTickOnLoad(true);
	}
}

BlockStationary::~BlockStationary()
{
}

void BlockStationary::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	BlockFluids::onNeighborBlockChange(var1, var2, var3, var4, var5);
	if(var1->getBlockId(var2, var3, var4) == this->blockID) {
	this->setNotStationary(var1, var2, var3, var4);
	}
}

void BlockStationary::setNotStationary(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	var1->editingBlocks = true;
	var1->setBlockAndMetadata(var2, var3, var4, this->blockID - 1, var5);
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID - 1);
	var1->editingBlocks = false;
}

void BlockStationary::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(this->blockMaterial == Material::s_lava) {
	int var6 = var5->nextInt(3);
	for(int var7 = 0; var7 < var6; ++var7) {
	var2 += var5->nextInt(3) - 1;
	++var3;
	var4 += var5->nextInt(3) - 1;
	int var8 = var1->getBlockId(var2, var3, var4);
	if(var8 == 0) {
	if(this->isBlockBurning(var1, var2 - 1, var3, var4) || this->isBlockBurning(var1, var2 + 1, var3, var4) || this->isBlockBurning(var1, var2, var3, var4 - 1) || this->isBlockBurning(var1, var2, var3, var4 + 1) || this->isBlockBurning(var1, var2, var3 - 1, var4) || this->isBlockBurning(var1, var2, var3 + 1, var4)) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_fire->blockID);
	return;
	}
	} else if(Block::s_blocksList[var8]->blockMaterial->blocksMovement()) {
	return;
	}
	}
	}
}

bool BlockStationary::isBlockBurning(World* var1, int var2, int var3, int var4)
{
	return var1->getBlockMaterial(var2, var3, var4)->getBurning();
}

void BlockStationary::initStatics()
{
}
