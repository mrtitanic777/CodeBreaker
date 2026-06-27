#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockPortal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockPortal.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockBreakable.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
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

BlockPortal::BlockPortal(int var1, int var2)
	: BlockBreakable(var1, var2, Material::s_portal, false)
{
}

BlockPortal::~BlockPortal()
{
}

AxisAlignedBB* BlockPortal::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

void BlockPortal::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4)
{
	float var5;
	float var6;
	if(var1->getBlockId(var2 - 1, var3, var4) != this->blockID && var1->getBlockId(var2 + 1, var3, var4) != this->blockID) {
	var5 = 2.0F / 16.0F;
	var6 = 0.5F;
	this->setBlockBounds(0.5F - var5, 0.0F, 0.5F - var6, 0.5F + var5, 1.0F, 0.5F + var6);
	} else {
	var5 = 0.5F;
	var6 = 2.0F / 16.0F;
	this->setBlockBounds(0.5F - var5, 0.0F, 0.5F - var6, 0.5F + var5, 1.0F, 0.5F + var6);
	}
}

bool BlockPortal::isOpaqueCube()
{
	return false;
}

bool BlockPortal::renderAsNormalBlock()
{
	return false;
}

bool BlockPortal::tryToCreatePortal(World* var1, int var2, int var3, int var4)
{
	unsigned char var5 = 0;
	unsigned char var6 = 0;
	if(var1->getBlockId(var2 - 1, var3, var4) == Block::s_obsidian->blockID || var1->getBlockId(var2 + 1, var3, var4) == Block::s_obsidian->blockID) {
	var5 = 1;
	}
	if(var1->getBlockId(var2, var3, var4 - 1) == Block::s_obsidian->blockID || var1->getBlockId(var2, var3, var4 + 1) == Block::s_obsidian->blockID) {
	var6 = 1;
	}
	System::out.println((JCat() + var5 + ", " + var6));
	if(var5 == var6) {
	return false;
	} else {
	if(var1->getBlockId(var2 - var5, var3, var4 - var6) == 0) {
	var2 -= var5;
	var4 -= var6;
	}
	int var7;
	int var8;
	for(var7 = -1; var7 <= 2; ++var7) {
	for(var8 = -1; var8 <= 3; ++var8) {
	bool var9 = var7 == -1 || var7 == 2 || var8 == -1 || var8 == 3;
	if(var7 != -1 && var7 != 2 || var8 != -1 && var8 != 3) {
	int var10 = var1->getBlockId(var2 + var5 * var7, var3 + var8, var4 + var6 * var7);
	if(var9) {
	if(var10 != Block::s_obsidian->blockID) {
	return false;
	}
	} else if(var10 != 0 && var10 != Block::s_fire->blockID) {
	return false;
	}
	}
	}
	}
	var1->editingBlocks = true;
	for(var7 = 0; var7 < 2; ++var7) {
	for(var8 = 0; var8 < 3; ++var8) {
	var1->setBlockWithNotify(var2 + var5 * var7, var3 + var8, var4 + var6 * var7, Block::s_portal->blockID);
	}
	}
	var1->editingBlocks = false;
	return true;
	}
}

void BlockPortal::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	unsigned char var6 = 0;
	unsigned char var7 = 1;
	if(var1->getBlockId(var2 - 1, var3, var4) == this->blockID || var1->getBlockId(var2 + 1, var3, var4) == this->blockID) {
	var6 = 1;
	var7 = 0;
	}
	int var8;
	for(var8 = var3; var1->getBlockId(var2, var8 - 1, var4) == this->blockID; --var8) {
	}
	if(var1->getBlockId(var2, var8 - 1, var4) != Block::s_obsidian->blockID) {
	var1->setBlockWithNotify(var2, var3, var4, 0);
	} else {
	int var9;
	for(var9 = 1; var9 < 4 && var1->getBlockId(var2, var8 + var9, var4) == this->blockID; ++var9) {
	}
	if(var9 == 3 && var1->getBlockId(var2, var8 + var9, var4) == Block::s_obsidian->blockID) {
	bool var10 = var1->getBlockId(var2 - 1, var3, var4) == this->blockID || var1->getBlockId(var2 + 1, var3, var4) == this->blockID;
	bool var11 = var1->getBlockId(var2, var3, var4 - 1) == this->blockID || var1->getBlockId(var2, var3, var4 + 1) == this->blockID;
	if(var10 && var11) {
	var1->setBlockWithNotify(var2, var3, var4, 0);
	} else if((var1->getBlockId(var2 + var6, var3, var4 + var7) != Block::s_obsidian->blockID || var1->getBlockId(var2 - var6, var3, var4 - var7) != this->blockID) && (var1->getBlockId(var2 - var6, var3, var4 - var7) != Block::s_obsidian->blockID || var1->getBlockId(var2 + var6, var3, var4 + var7) != this->blockID)) {
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
	} else {
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
	}
}

bool BlockPortal::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return true;
}

int BlockPortal::quantityDropped(Random* var1)
{
	return 0;
}

int BlockPortal::getRenderBlockPass()
{
	return 1;
}

void BlockPortal::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5)
{
	if(!var1->multiplayerWorld) {
	var5->setInPortal();
	}
}

void BlockPortal::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var5->nextInt(100) == 0) {
	var1->playSoundEffect((double)var2 + 0.5,(double)var3 + 0.5,(double)var4 + 0.5,"portal.portal",1.0F,var5->nextFloat() * 0.4F + 0.8F);
	}
	for(int var6 = 0; var6 < 4; ++var6) {
	double var7 = (double)((float)var2 + var5->nextFloat());
	double var9 = (double)((float)var3 + var5->nextFloat());
	double var11 = (double)((float)var4 + var5->nextFloat());
	double var13 = 0.0;
	double var15 = 0.0;
	double var17 = 0.0;
	int var19 = var5->nextInt(2) * 2 - 1;
	var13 = ((double)var5->nextFloat() - 0.5) * 0.5;
	var15 = ((double)var5->nextFloat() - 0.5) * 0.5;
	var17 = ((double)var5->nextFloat() - 0.5) * 0.5;
	if(var1->getBlockId(var2 - 1, var3, var4) != this->blockID && var1->getBlockId(var2 + 1, var3, var4) != this->blockID) {
	var7 = (double)var2 + 0.5 + 0.25 * (double)var19;
	var13 = (double)(var5->nextFloat() * 2.0F * (float)var19);
	} else {
	var11 = (double)var4 + 0.5 + 0.25 * (double)var19;
	var17 = (double)(var5->nextFloat() * 2.0F * (float)var19);
	}
	var1->spawnParticle("portal", var7, var9, var11, var13, var15, var17);
	}
}

void BlockPortal::initStatics()
{
}
