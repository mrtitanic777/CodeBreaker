#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockChest.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockContainer.h"
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
#include "IInventory.h"
#include "InventoryLargeChest.h"
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
#include "TileEntityChest.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

BlockChest::BlockChest(int var1)
	: BlockContainer(var1, Material::s_wood)
	, rand(NULL)
{
	this->rand = new Random();
	this->blockIndexInTexture = 26;
}

BlockChest::~BlockChest()
{
}

int BlockChest::getBlockTexture(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	if(var5 == 1) {
	return this->blockIndexInTexture - 1;
	} else if(var5 == 0) {
	return this->blockIndexInTexture - 1;
	} else {
	int var6 = var1->getBlockId(var2, var3, var4 - 1);
	int var7 = var1->getBlockId(var2, var3, var4 + 1);
	int var8 = var1->getBlockId(var2 - 1, var3, var4);
	int var9 = var1->getBlockId(var2 + 1, var3, var4);
	int var10;
	int var11;
	int var12;
	unsigned char var13;
	if(var6 != this->blockID && var7 != this->blockID) {
	if(var8 != this->blockID && var9 != this->blockID) {
	unsigned char var14 = 3;
	if(Block::s_opaqueCubeLookup[var6] && !Block::s_opaqueCubeLookup[var7]) {
	var14 = 3;
	}
	if(Block::s_opaqueCubeLookup[var7] && !Block::s_opaqueCubeLookup[var6]) {
	var14 = 2;
	}
	if(Block::s_opaqueCubeLookup[var8] && !Block::s_opaqueCubeLookup[var9]) {
	var14 = 5;
	}
	if(Block::s_opaqueCubeLookup[var9] && !Block::s_opaqueCubeLookup[var8]) {
	var14 = 4;
	}
	return var5 == var14 ? this->blockIndexInTexture + 1 : this->blockIndexInTexture;
	} else if(var5 != 4 && var5 != 5) {
	var10 = 0;
	if(var8 == this->blockID) {
	var10 = -1;
	}
	var11 = var1->getBlockId(var8 == this->blockID ? var2 - 1 : var2 + 1, var3, var4 - 1);
	var12 = var1->getBlockId(var8 == this->blockID ? var2 - 1 : var2 + 1, var3, var4 + 1);
	if(var5 == 3) {
	var10 = -1 - var10;
	}
	var13 = 3;
	if((Block::s_opaqueCubeLookup[var6] || Block::s_opaqueCubeLookup[var11]) && !Block::s_opaqueCubeLookup[var7] && !Block::s_opaqueCubeLookup[var12]) {
	var13 = 3;
	}
	if((Block::s_opaqueCubeLookup[var7] || Block::s_opaqueCubeLookup[var12]) && !Block::s_opaqueCubeLookup[var6] && !Block::s_opaqueCubeLookup[var11]) {
	var13 = 2;
	}
	return (var5 == var13 ? this->blockIndexInTexture + 16 : this->blockIndexInTexture + 32) + var10;
	} else {
	return this->blockIndexInTexture;
	}
	} else if(var5 != 2 && var5 != 3) {
	var10 = 0;
	if(var6 == this->blockID) {
	var10 = -1;
	}
	var11 = var1->getBlockId(var2 - 1, var3, var6 == this->blockID ? var4 - 1 : var4 + 1);
	var12 = var1->getBlockId(var2 + 1, var3, var6 == this->blockID ? var4 - 1 : var4 + 1);
	if(var5 == 4) {
	var10 = -1 - var10;
	}
	var13 = 5;
	if((Block::s_opaqueCubeLookup[var8] || Block::s_opaqueCubeLookup[var11]) && !Block::s_opaqueCubeLookup[var9] && !Block::s_opaqueCubeLookup[var12]) {
	var13 = 5;
	}
	if((Block::s_opaqueCubeLookup[var9] || Block::s_opaqueCubeLookup[var12]) && !Block::s_opaqueCubeLookup[var8] && !Block::s_opaqueCubeLookup[var11]) {
	var13 = 4;
	}
	return (var5 == var13 ? this->blockIndexInTexture + 16 : this->blockIndexInTexture + 32) + var10;
	} else {
	return this->blockIndexInTexture;
	}
	}
}

int BlockChest::getBlockTextureFromSide(int var1)
{
	return var1 == 1 ? this->blockIndexInTexture - 1 : (var1 == 0 ? this->blockIndexInTexture - 1 : (var1 == 3 ? this->blockIndexInTexture + 1 : this->blockIndexInTexture));
}

bool BlockChest::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	int var5 = 0;
	if(var1->getBlockId(var2 - 1, var3, var4) == this->blockID) {
	++var5;
	}
	if(var1->getBlockId(var2 + 1, var3, var4) == this->blockID) {
	++var5;
	}
	if(var1->getBlockId(var2, var3, var4 - 1) == this->blockID) {
	++var5;
	}
	if(var1->getBlockId(var2, var3, var4 + 1) == this->blockID) {
	++var5;
	}
	return var5 > 1 ? false : (this->isThereANeighborChest(var1, var2 - 1, var3, var4) ? false : (this->isThereANeighborChest(var1, var2 + 1, var3, var4) ? false : (this->isThereANeighborChest(var1, var2, var3, var4 - 1) ? false : !this->isThereANeighborChest(var1, var2, var3, var4 + 1))));
}

bool BlockChest::isThereANeighborChest(World* var1, int var2, int var3, int var4)
{
	return var1->getBlockId(var2, var3, var4) != this->blockID ? false : (var1->getBlockId(var2 - 1, var3, var4) == this->blockID ? true : (var1->getBlockId(var2 + 1, var3, var4) == this->blockID ? true : (var1->getBlockId(var2, var3, var4 - 1) == this->blockID ? true : var1->getBlockId(var2, var3, var4 + 1) == this->blockID)));
}

void BlockChest::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	TileEntityChest* var5 = (TileEntityChest*)var1->getBlockTileEntity(var2, var3, var4);
	for(int var6 = 0; var6 < var5->getSizeInventory(); ++var6) {
	ItemStack* var7 = var5->getStackInSlot(var6);
	if(var7 != NULL) {
	float var8 = this->rand->nextFloat() * 0.8F + 0.1F;
	float var9 = this->rand->nextFloat() * 0.8F + 0.1F;
	float var10 = this->rand->nextFloat() * 0.8F + 0.1F;
	while(var7->stackSize > 0) {
	int var11 = this->rand->nextInt(21) + 10;
	if(var11 > var7->stackSize) {
	var11 = var7->stackSize;
	}
	var7->stackSize -= var11;
	EntityItem* var12 = new EntityItem(var1, (double)((float)var2 + var8), (double)((float)var3 + var9), (double)((float)var4 + var10), new ItemStack(var7->itemID, var11, var7->itemDamage));
	float var13 = 0.05F;
	var12->motionX = (double)((float)this->rand->nextGaussian() * var13);
	var12->motionY = (double)((float)this->rand->nextGaussian() * var13 + 0.2F);
	var12->motionZ = (double)((float)this->rand->nextGaussian() * var13);
	var1->entityJoinedWorld(var12);
	}
	}
	}
	BlockContainer::onBlockRemoval(var1, var2, var3, var4);
}

bool BlockChest::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	JObject var6 = (TileEntityChest*)var1->getBlockTileEntity(var2, var3, var4);
	if(var1->isBlockOpaqueCube(var2, var3 + 1, var4)) {
	return true;
	} else if(var1->getBlockId(var2 - 1, var3, var4) == this->blockID && var1->isBlockOpaqueCube(var2 - 1, var3 + 1, var4)) {
	return true;
	} else if(var1->getBlockId(var2 + 1, var3, var4) == this->blockID && var1->isBlockOpaqueCube(var2 + 1, var3 + 1, var4)) {
	return true;
	} else if(var1->getBlockId(var2, var3, var4 - 1) == this->blockID && var1->isBlockOpaqueCube(var2, var3 + 1, var4 - 1)) {
	return true;
	} else if(var1->getBlockId(var2, var3, var4 + 1) == this->blockID && var1->isBlockOpaqueCube(var2, var3 + 1, var4 + 1)) {
	return true;
	} else {
	if(var1->getBlockId(var2 - 1, var3, var4) == this->blockID) {
	var6 = new InventoryLargeChest("Large chest", (TileEntityChest*)var1->getBlockTileEntity(var2 - 1, var3, var4), (IInventory*)var6);
	}
	if(var1->getBlockId(var2 + 1, var3, var4) == this->blockID) {
	var6 = new InventoryLargeChest("Large chest",(IInventory*)var6,(TileEntityChest*)var1->getBlockTileEntity(var2 + 1,var3,var4));
	}
	if(var1->getBlockId(var2, var3, var4 - 1) == this->blockID) {
	var6 = new InventoryLargeChest("Large chest", (TileEntityChest*)var1->getBlockTileEntity(var2, var3, var4 - 1), (IInventory*)var6);
	}
	if(var1->getBlockId(var2, var3, var4 + 1) == this->blockID) {
	var6 = new InventoryLargeChest("Large chest",(IInventory*)var6,(TileEntityChest*)var1->getBlockTileEntity(var2,var3,var4 + 1));
	}
	var5->displayGUIChest((IInventory*)var6);
	return true;
	}
}

TileEntity* BlockChest::SetBlockEntity()
{
	return new TileEntityChest();
}

void BlockChest::initStatics()
{
}
