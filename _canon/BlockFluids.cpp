#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockFluids.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFluids.h"
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

BlockFluids::BlockFluids(int var1, Material* var2)
	: Block(var1, (var2 == Material::s_lava ? 14 : 12) * 16 + 13, var2)
{
	float var3 = 0.0F;
	float var4 = 0.0F;
	this->setBlockBounds(0.0F + var4, 0.0F + var3, 0.0F + var4, 1.0F + var4, 1.0F + var3, 1.0F + var4);
	this->setTickOnLoad(true);
}

BlockFluids::~BlockFluids()
{
}

float BlockFluids::getFluidHeightPercent(int var0)
{
	if(var0 >= 8) {
	var0 = 0;
	}
	float var1 = (float)(var0 + 1) / 9.0F;
	return var1;
}

int BlockFluids::getBlockTextureFromSide(int var1)
{
	return var1 != 0 && var1 != 1 ? this->blockIndexInTexture + 1 : this->blockIndexInTexture;
}

int BlockFluids::getEffectiveFlowDecay(World* var1, int var2, int var3, int var4)
{
	return var1->getBlockMaterial(var2, var3, var4) != this->blockMaterial ? -1 : var1->getBlockMetadata(var2, var3, var4);
}

int BlockFluids::getFluidLevel(IBlockAccess* var1, int var2, int var3, int var4)
{
	if(var1->getBlockMaterial(var2, var3, var4) != this->blockMaterial) {
	return -1;
	} else {
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	if(var5 >= 8) {
	var5 = 0;
	}
	return var5;
	}
}

bool BlockFluids::renderAsNormalBlock()
{
	return false;
}

bool BlockFluids::isOpaqueCube()
{
	return false;
}

bool BlockFluids::canCollideCheck(int var1, bool var2)
{
	return var2 && var1 == 0;
}

bool BlockFluids::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	Material* var6 = var1->getBlockMaterial(var2, var3, var4);
	return var6 == this->blockMaterial ? false : (var6 == Material::s_ice ? false : (var5 == 1 ? true : Block::shouldSideBeRendered(var1, var2, var3, var4, var5)));
}

AxisAlignedBB* BlockFluids::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

int BlockFluids::getRenderType()
{
	return 4;
}

int BlockFluids::idDropped(int var1, Random* var2)
{
	return 0;
}

int BlockFluids::quantityDropped(Random* var1)
{
	return 0;
}

Vec3D* BlockFluids::getFlowVector(IBlockAccess* var1, int var2, int var3, int var4)
{
	Vec3D* var5 = Vec3D::createVector(0.0, 0.0, 0.0);
	int var6 = this->getFluidLevel(var1, var2, var3, var4);
	for(int var7 = 0; var7 < 4; ++var7) {
	int var8 = var2;
	int var10 = var4;
	if(var7 == 0) {
	var8 = var2 - 1;
	}
	if(var7 == 1) {
	var10 = var4 - 1;
	}
	if(var7 == 2) {
	++var8;
	}
	if(var7 == 3) {
	++var10;
	}
	int var11 = this->getFluidLevel(var1, var8, var3, var10);
	int var12;
	if(var11 < 0) {
	if(!var1->getBlockMaterial(var8, var3, var10)->blocksMovement()) {
	var11 = this->getFluidLevel(var1, var8, var3 - 1, var10);
	if(var11 >= 0) {
	var12 = var11 - (var6 - 8);
	var5 = var5->addVector((double)((var8 - var2) * var12), (double)((var3 - var3) * var12), (double)((var10 - var4) * var12));
	}
	}
	} else if(var11 >= 0) {
	var12 = var11 - var6;
	var5 = var5->addVector((double)((var8 - var2) * var12), (double)((var3 - var3) * var12), (double)((var10 - var4) * var12));
	}
	}
	if(var1->getBlockMetadata(var2, var3, var4) >= 8) {
	bool var13 = false;
	if(var13 || this->shouldSideBeRendered(var1, var2, var3, var4 - 1, 2)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2, var3, var4 + 1, 3)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2 - 1, var3, var4, 4)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2 + 1, var3, var4, 5)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2, var3 + 1, var4 - 1, 2)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2, var3 + 1, var4 + 1, 3)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2 - 1, var3 + 1, var4, 4)) {
	var13 = true;
	}
	if(var13 || this->shouldSideBeRendered(var1, var2 + 1, var3 + 1, var4, 5)) {
	var13 = true;
	}
	if(var13) {
	var5 = var5->normalize()->addVector(0.0, -6.0, 0.0);
	}
	}
	var5 = var5->normalize();
	return var5;
}

void BlockFluids::velocityToAddToEntity(World* var1, int var2, int var3, int var4, Entity* var5, Vec3D* var6)
{
	Vec3D* var7 = this->getFlowVector(var1, var2, var3, var4);
	var6->xCoord += var7->xCoord;
	var6->yCoord += var7->yCoord;
	var6->zCoord += var7->zCoord;
}

int BlockFluids::tickRate()
{
	return this->blockMaterial == Material::s_water ? 5 : (this->blockMaterial == Material::s_lava ? 30 : 0);
}

float BlockFluids::getBlockBrightness(IBlockAccess* var1, int var2, int var3, int var4)
{
	float var5 = var1->getLightBrightness(var2, var3, var4);
	float var6 = var1->getLightBrightness(var2, var3 + 1, var4);
	return var5 > var6 ? var5 : var6;
}

void BlockFluids::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	Block::updateTick(var1, var2, var3, var4, var5);
}

int BlockFluids::getRenderBlockPass()
{
	return this->blockMaterial == Material::s_water ? 1 : 0;
}

void BlockFluids::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(this->blockMaterial == Material::s_water && var5->nextInt(64) == 0) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 > 0 && var6 < 8) {
	var1->playSoundEffect((double)((float)var2 + 0.5F),(double)((float)var3 + 0.5F),(double)((float)var4 + 0.5F),"liquid.water",var5->nextFloat() * 0.25F + 12.0F / 16.0F,var5->nextFloat() * 1.0F + 0.5F);
	}
	}
	if(this->blockMaterial == Material::s_lava && var1->getBlockMaterial(var2, var3 + 1, var4) == Material::s_air && !var1->isBlockOpaqueCube(var2, var3 + 1, var4) && var5->nextInt(100) == 0) {
	double var12 = (double)((float)var2 + var5->nextFloat());
	double var8 = (double)var3 + this->maxY;
	double var10 = (double)((float)var4 + var5->nextFloat());
	var1->spawnParticle("lava", var12, var8, var10, 0.0, 0.0, 0.0);
	}
}

double BlockFluids::getFlowDirection(IBlockAccess* var0, int var1, int var2, int var3, Material* var4)
{
	Vec3D* var5 = NULL;
	if(var4 == Material::s_water) {
	var5 = ((BlockFluids*)Block::s_waterStill)->getFlowVector(var0, var1, var2, var3);
	}
	if(var4 == Material::s_lava) {
	var5 = ((BlockFluids*)Block::s_lavaStill)->getFlowVector(var0, var1, var2, var3);
	}
	return var5->xCoord == 0.0 && var5->zCoord == 0.0 ? -1000.0 : atan2f(var5->zCoord, var5->xCoord) - 3.1415927F * 0.5;
}

void BlockFluids::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	this->checkForHarden(var1, var2, var3, var4);
}

void BlockFluids::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	this->checkForHarden(var1, var2, var3, var4);
}

void BlockFluids::checkForHarden(World* var1, int var2, int var3, int var4)
{
	if(var1->getBlockId(var2, var3, var4) == this->blockID) {
	if(this->blockMaterial == Material::s_lava) {
	bool var5 = false;
	if(var5 || var1->getBlockMaterial(var2, var3, var4 - 1) == Material::s_water) {
	var5 = true;
	}
	if(var5 || var1->getBlockMaterial(var2, var3, var4 + 1) == Material::s_water) {
	var5 = true;
	}
	if(var5 || var1->getBlockMaterial(var2 - 1, var3, var4) == Material::s_water) {
	var5 = true;
	}
	if(var5 || var1->getBlockMaterial(var2 + 1, var3, var4) == Material::s_water) {
	var5 = true;
	}
	if(var5 || var1->getBlockMaterial(var2, var3 + 1, var4) == Material::s_water) {
	var5 = true;
	}
	if(var5) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 == 0) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_obsidian->blockID);
	} else if(var6 <= 4) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_cobblestone->blockID);
	}
	this->triggerLavaMixEffects(var1, var2, var3, var4);
	}
	}
	}
}

void BlockFluids::triggerLavaMixEffects(World* var1, int var2, int var3, int var4)
{
	var1->playSoundEffect((double)((float)var2 + 0.5F),(double)((float)var3 + 0.5F),(double)((float)var4 + 0.5F),"random.fizz",0.5F,2.6F + (var1->rand->nextFloat() - var1->rand->nextFloat()) * 0.8F);
	for(int var5 = 0; var5 < 8; ++var5) {
	var1->spawnParticle("largesmoke",(double)var2 + javaMathRandom(),(double)var3 + 1.2,(double)var4 + javaMathRandom(),0.0,0.0,0.0);
	}
}

void BlockFluids::initStatics()
{
}
