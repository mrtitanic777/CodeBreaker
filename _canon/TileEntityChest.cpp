#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntityChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityChest.h"
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
#include "IInventory.h"
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
#include "Slot.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

TileEntityChest::TileEntityChest()
{
	this->chestContents = JArray<ItemStack* >(36);
}

TileEntityChest::~TileEntityChest()
{
}

int TileEntityChest::getSizeInventory()
{
	return 27;
}

ItemStack* TileEntityChest::getStackInSlot(int var1)
{
	return this->chestContents[var1];
}

ItemStack* TileEntityChest::decrStackSize(int var1, int var2)
{
	if(this->chestContents[var1] != NULL) {
	ItemStack* var3;
	if(this->chestContents[var1]->stackSize <= var2) {
	var3 = this->chestContents[var1];
	this->chestContents[var1] = NULL;
	this->onInventoryChanged();
	return var3;
	} else {
	var3 = this->chestContents[var1]->splitStack(var2);
	if(this->chestContents[var1]->stackSize == 0) {
	this->chestContents[var1] = NULL;
	}
	this->onInventoryChanged();
	return var3;
	}
	} else {
	return NULL;
	}
}

void TileEntityChest::setInventorySlotContents(int var1, ItemStack* var2)
{
	this->chestContents[var1] = var2;
	if(var2 != NULL && var2->stackSize > this->getInventoryStackLimit()) {
	var2->stackSize = this->getInventoryStackLimit();
	}
	this->onInventoryChanged();
}

JavaString TileEntityChest::getInvName()
{
	return "Chest";
}

void TileEntityChest::readFromNBT(NBTTagCompound* var1)
{
	TileEntity::readFromNBT(var1);
	NBTTagList* var2 = var1->getTagList("Items");
	this->chestContents = JArray<ItemStack* >(this->getSizeInventory());
	for(int var3 = 0; var3 < var2->tagCount(); ++var3) {
	NBTTagCompound* var4 = (NBTTagCompound*)var2->tagAt(var3);
	int var5 = var4->getByte("Slot") & 255;
	if(var5 >= 0 && var5 < this->chestContents.length) {
	this->chestContents[var5] = new ItemStack(var4);
	}
	}
}

void TileEntityChest::writeToNBT(NBTTagCompound* var1)
{
	TileEntity::writeToNBT(var1);
	NBTTagList* var2 = new NBTTagList();
	for(int var3 = 0; var3 < this->chestContents.length; ++var3) {
	if(this->chestContents[var3] != NULL) {
	NBTTagCompound* var4 = new NBTTagCompound();
	var4->setByte("Slot", (unsigned char)var3);
	if (this->chestContents[var3] != NULL) { this->chestContents[var3]->writeToNBT(var4); }
	var2->setTag(var4);
	}
	}
	var1->setTag("Items", var2);
}

int TileEntityChest::getInventoryStackLimit()
{
	return 64;
}

void TileEntityChest::onInventoryChanged()
{
	TileEntity::onInventoryChanged();
}

void TileEntityChest::initStatics()
{
}
