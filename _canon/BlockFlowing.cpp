#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockFlowing.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlowing.h"
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

BlockFlowing::BlockFlowing(int var1, Material* var2)
	: BlockFluids(var1, var2)
	, numAdjacentSources(0)
{
	this->numAdjacentSources = 0;
	this->optimalFlowDirections = JArray<bool >(4);
	this->flowCost = JArray<int >(4);
}

BlockFlowing::~BlockFlowing()
{
}

void BlockFlowing::setStationary(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	var1->setBlockAndMetadata(var2, var3, var4, this->blockID + 1, var5);
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
	var1->markBlockNeedsUpdate(var2, var3, var4);
}

void BlockFlowing::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	int var6 = this->getEffectiveFlowDecay(var1, var2, var3, var4);
	unsigned char var7 = 1;
	if(this->blockMaterial == Material::s_lava && !var1->worldProvider->doesWaterVaporize) {
	var7 = 2;
	}
	bool var8 = true;
	int var10;
	if(var6 > 0) {
	unsigned char var9 = -100;
	this->numAdjacentSources = 0;
	int var12 = this->getSmallestFlowDecay(var1, var2 - 1, var3, var4, var9);
	var12 = this->getSmallestFlowDecay(var1, var2 + 1, var3, var4, var12);
	var12 = this->getSmallestFlowDecay(var1, var2, var3, var4 - 1, var12);
	var12 = this->getSmallestFlowDecay(var1, var2, var3, var4 + 1, var12);
	var10 = var12 + var7;
	if(var10 >= 8 || var12 < 0) {
	var10 = -1;
	}
	if(this->getEffectiveFlowDecay(var1, var2, var3 + 1, var4) >= 0) {
	int var11 = this->getEffectiveFlowDecay(var1, var2, var3 + 1, var4);
	if(var11 >= 8) {
	var10 = var11;
	} else {
	var10 = var11 + 8;
	}
	}
	if(this->numAdjacentSources >= 2 && this->blockMaterial == Material::s_water) {
	if(var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
	var10 = 0;
	} else if(var1->getBlockMaterial(var2, var3 - 1, var4) == this->blockMaterial && var1->getBlockMetadata(var2, var3, var4) == 0) {
	var10 = 0;
	}
	}
	if(this->blockMaterial == Material::s_lava && var6 < 8 && var10 < 8 && var10 > var6 && var5->nextInt(4) != 0) {
	var10 = var6;
	var8 = false;
	}
	if(var10 != var6) {
	var6 = var10;
	if(var10 < 0) {
	var1->setBlockWithNotify(var2, var3, var4, 0);
	} else {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var10);
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	}
	} else if(var8) {
	this->setStationary(var1, var2, var3, var4);
	}
	} else {
	this->setStationary(var1, var2, var3, var4);
	}
	if(this->canFlowInto(var1, var2, var3 - 1, var4)) {
	if(var6 >= 8) {
	var1->setBlockAndMetadataWithNotify(var2, var3 - 1, var4, this->blockID, var6);
	} else {
	var1->setBlockAndMetadataWithNotify(var2, var3 - 1, var4, this->blockID, var6 + 8);
	}
	} else if(var6 >= 0 && (var6 == 0 || this->isBlockBlockingFlow(var1, var2, var3 - 1, var4))) {
	JArray<bool > var13 = this->getOptimalFlowDirections(var1, var2, var3, var4);
	var10 = var6 + var7;
	if(var6 >= 8) {
	var10 = 1;
	}
	if(var10 >= 8) {
	return;
	}
	if(var13[0]) {
	this->flowIntoBlock(var1, var2 - 1, var3, var4, var10);
	}
	if(var13[1]) {
	this->flowIntoBlock(var1, var2 + 1, var3, var4, var10);
	}
	if(var13[2]) {
	this->flowIntoBlock(var1, var2, var3, var4 - 1, var10);
	}
	if(var13[3]) {
	this->flowIntoBlock(var1, var2, var3, var4 + 1, var10);
	}
	}
}

void BlockFlowing::flowIntoBlock(World* var1, int var2, int var3, int var4, int var5)
{
	if(this->canFlowInto(var1, var2, var3, var4)) {
	int var6 = var1->getBlockId(var2, var3, var4);
	if(var6 > 0) {
	if(this->blockMaterial == Material::s_lava) {
	this->triggerLavaMixEffects(var1, var2, var3, var4);
	} else {
	if (Block::s_blocksList[var6] != NULL) { Block::s_blocksList[var6]->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4)); }
	}
	}
	var1->setBlockAndMetadataWithNotify(var2, var3, var4, this->blockID, var5);
	}
}

int BlockFlowing::calculateFlowCost(World* var1, int var2, int var3, int var4, int var5, int var6)
{
	int var7 = 1000;
	for(int var8 = 0; var8 < 4; ++var8) {
	if((var8 != 0 || var6 != 1) && (var8 != 1 || var6 != 0) && (var8 != 2 || var6 != 3) && (var8 != 3 || var6 != 2)) {
	int var9 = var2;
	int var11 = var4;
	if(var8 == 0) {
	var9 = var2 - 1;
	}
	if(var8 == 1) {
	++var9;
	}
	if(var8 == 2) {
	var11 = var4 - 1;
	}
	if(var8 == 3) {
	++var11;
	}
	if(!this->isBlockBlockingFlow(var1, var9, var3, var11) && (var1->getBlockMaterial(var9, var3, var11) != this->blockMaterial || var1->getBlockMetadata(var9, var3, var11) != 0)) {
	if(!this->isBlockBlockingFlow(var1, var9, var3 - 1, var11)) {
	return var5;
	}
	if(var5 < 4) {
	int var12 = this->calculateFlowCost(var1, var9, var3, var11, var5 + 1, var8);
	if(var12 < var7) {
	var7 = var12;
	}
	}
	}
	}
	}
	return var7;
}

JArray<bool > BlockFlowing::getOptimalFlowDirections(World* var1, int var2, int var3, int var4)
{
	int var5;
	int var6;
	for(var5 = 0; var5 < 4; ++var5) {
	this->flowCost[var5] = 1000;
	var6 = var2;
	int var8 = var4;
	if(var5 == 0) {
	var6 = var2 - 1;
	}
	if(var5 == 1) {
	++var6;
	}
	if(var5 == 2) {
	var8 = var4 - 1;
	}
	if(var5 == 3) {
	++var8;
	}
	if(!this->isBlockBlockingFlow(var1, var6, var3, var8) && (var1->getBlockMaterial(var6, var3, var8) != this->blockMaterial || var1->getBlockMetadata(var6, var3, var8) != 0)) {
	if(!this->isBlockBlockingFlow(var1, var6, var3 - 1, var8)) {
	this->flowCost[var5] = 0;
	} else {
	this->flowCost[var5] = this->calculateFlowCost(var1, var6, var3, var8, 1, var5);
	}
	}
	}
	var5 = this->flowCost[0];
	for(var6 = 1; var6 < 4; ++var6) {
	if(this->flowCost[var6] < var5) {
	var5 = this->flowCost[var6];
	}
	}
	for(var6 = 0; var6 < 4; ++var6) {
	this->optimalFlowDirections[var6] = this->flowCost[var6] == var5;
	}
	return this->optimalFlowDirections;
}

bool BlockFlowing::isBlockBlockingFlow(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockId(var2, var3, var4);
	if(var5 != Block::s_doorWood->blockID && var5 != Block::s_doorSteel->blockID && var5 != Block::s_signPost->blockID && var5 != Block::s_ladder->blockID && var5 != Block::s_reed->blockID) {
	if(var5 == 0) {
	return false;
	} else {
	Material* var6 = Block::s_blocksList[var5]->blockMaterial;
	return var6->isSolid();
	}
	} else {
	return true;
	}
}

int BlockFlowing::getSmallestFlowDecay(World* var1, int var2, int var3, int var4, int var5)
{
	int var6 = this->getEffectiveFlowDecay(var1, var2, var3, var4);
	if(var6 < 0) {
	return var5;
	} else {
	if(var6 == 0) {
	++this->numAdjacentSources;
	}
	if(var6 >= 8) {
	var6 = 0;
	}
	return var5 >= 0 && var6 >= var5 ? var5 : var6;
	}
}

bool BlockFlowing::canFlowInto(World* var1, int var2, int var3, int var4)
{
	Material* var5 = var1->getBlockMaterial(var2, var3, var4);
	return var5 == this->blockMaterial ? false : (var5 == Material::s_lava ? false : !this->isBlockBlockingFlow(var1, var2, var3, var4));
}

void BlockFlowing::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	BlockFluids::onBlockAdded(var1, var2, var3, var4);
	if(var1->getBlockId(var2, var3, var4) == this->blockID) {
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID);
	}
}

void BlockFlowing::initStatics()
{
}
