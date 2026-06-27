#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from TileEntityFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityFurnace.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFurnace.h"
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

TileEntityFurnace::TileEntityFurnace()
	: furnaceBurnTime(0)
	, currentItemBurnTime(0)
	, furnaceCookTime(0)
{
	this->furnaceItemStacks = JArray<ItemStack* >(3);
	this->furnaceBurnTime = 0;
	this->currentItemBurnTime = 0;
	this->furnaceCookTime = 0;
}

TileEntityFurnace::~TileEntityFurnace()
{
}

int TileEntityFurnace::getSizeInventory()
{
	return this->furnaceItemStacks.length;
}

ItemStack* TileEntityFurnace::getStackInSlot(int var1)
{
	return this->furnaceItemStacks[var1];
}

ItemStack* TileEntityFurnace::decrStackSize(int var1, int var2)
{
	if(this->furnaceItemStacks[var1] != NULL) {
	ItemStack* var3;
	if(this->furnaceItemStacks[var1]->stackSize <= var2) {
	var3 = this->furnaceItemStacks[var1];
	this->furnaceItemStacks[var1] = NULL;
	return var3;
	} else {
	var3 = this->furnaceItemStacks[var1]->splitStack(var2);
	if(this->furnaceItemStacks[var1]->stackSize == 0) {
	this->furnaceItemStacks[var1] = NULL;
	}
	return var3;
	}
	} else {
	return NULL;
	}
}

void TileEntityFurnace::setInventorySlotContents(int var1, ItemStack* var2)
{
	this->furnaceItemStacks[var1] = var2;
	if(var2 != NULL && var2->stackSize > this->getInventoryStackLimit()) {
	var2->stackSize = this->getInventoryStackLimit();
	}
}

JavaString TileEntityFurnace::getInvName()
{
	return "Chest";
}

void TileEntityFurnace::readFromNBT(NBTTagCompound* var1)
{
	TileEntity::readFromNBT(var1);
	NBTTagList* var2 = var1->getTagList("Items");
	this->furnaceItemStacks = JArray<ItemStack* >(this->getSizeInventory());
	for(int var3 = 0; var3 < var2->tagCount(); ++var3) {
	NBTTagCompound* var4 = (NBTTagCompound*)var2->tagAt(var3);
	unsigned char var5 = var4->getByte("Slot");
	if(var5 >= 0 && var5 < this->furnaceItemStacks.length) {
	this->furnaceItemStacks[var5] = new ItemStack(var4);
	}
	}
	this->furnaceBurnTime = var1->getShort("BurnTime");
	this->furnaceCookTime = var1->getShort("CookTime");
	this->currentItemBurnTime = this->getItemBurnTime(this->furnaceItemStacks[1]);
}

void TileEntityFurnace::writeToNBT(NBTTagCompound* var1)
{
	TileEntity::writeToNBT(var1);
	var1->setShort("BurnTime", (short)this->furnaceBurnTime);
	var1->setShort("CookTime", (short)this->furnaceCookTime);
	NBTTagList* var2 = new NBTTagList();
	for(int var3 = 0; var3 < this->furnaceItemStacks.length; ++var3) {
	if(this->furnaceItemStacks[var3] != NULL) {
	NBTTagCompound* var4 = new NBTTagCompound();
	var4->setByte("Slot", (unsigned char)var3);
	if (this->furnaceItemStacks[var3] != NULL) { this->furnaceItemStacks[var3]->writeToNBT(var4); }
	var2->setTag(var4);
	}
	}
	var1->setTag("Items", var2);
}

int TileEntityFurnace::getInventoryStackLimit()
{
	return 64;
}

int TileEntityFurnace::getCookProgressScaled(int var1)
{
	return this->furnaceCookTime * var1 / 200;
}

int TileEntityFurnace::getBurnTimeRemainingScaled(int var1)
{
	if(this->currentItemBurnTime == 0) {
	this->currentItemBurnTime = 200;
	}
	return this->furnaceBurnTime * var1 / this->currentItemBurnTime;
}

bool TileEntityFurnace::isBurning()
{
	return this->furnaceBurnTime > 0;
}

void TileEntityFurnace::updateEntity()
{
	bool var1 = this->furnaceBurnTime > 0;
	bool var2 = false;
	if(this->furnaceBurnTime > 0) {
	--this->furnaceBurnTime;
	}
	if(!this->worldObj->multiplayerWorld) {
	if(this->furnaceBurnTime == 0 && this->canSmelt()) {
	this->currentItemBurnTime = this->furnaceBurnTime = this->getItemBurnTime(this->furnaceItemStacks[1]);
	if(this->furnaceBurnTime > 0) {
	var2 = true;
	if(this->furnaceItemStacks[1] != NULL) {
	--this->furnaceItemStacks[1]->stackSize;
	if(this->furnaceItemStacks[1]->stackSize == 0) {
	this->furnaceItemStacks[1] = NULL;
	}
	}
	}
	}
	if(this->isBurning() && this->canSmelt()) {
	++this->furnaceCookTime;
	if(this->furnaceCookTime == 200) {
	this->furnaceCookTime = 0;
	this->smeltItem();
	var2 = true;
	}
	} else {
	this->furnaceCookTime = 0;
	}
	if(var1 != this->furnaceBurnTime > 0) {
	var2 = true;
	BlockFurnace::updateFurnaceBlockState(this->furnaceBurnTime > 0, this->worldObj, this->xCoord, this->yCoord, this->zCoord);
	}
	}
	if(var2) {
	this->onInventoryChanged();
	}
}

bool TileEntityFurnace::canSmelt()
{
	if(this->furnaceItemStacks[0] == NULL) {
	return false;
	} else {
	int var1 = this->getSmeltingResultItem(this->furnaceItemStacks[0]->getItem()->shiftedIndex);
	return var1 < 0 ? false : (this->furnaceItemStacks[2] == NULL ? true : (this->furnaceItemStacks[2]->itemID != var1 ? false : (this->furnaceItemStacks[2]->stackSize < this->getInventoryStackLimit() && this->furnaceItemStacks[2]->stackSize < this->furnaceItemStacks[2]->getMaxStackSize() ? true : this->furnaceItemStacks[2]->stackSize < Item::s_itemsList[var1]->getItemStackLimit())));
	}
}

void TileEntityFurnace::smeltItem()
{
	if(this->canSmelt()) {
	int var1 = this->getSmeltingResultItem(this->furnaceItemStacks[0]->getItem()->shiftedIndex);
	if(this->furnaceItemStacks[2] == NULL) {
	this->furnaceItemStacks[2] = new ItemStack(var1, 1);
	} else if(this->furnaceItemStacks[2]->itemID == var1) {
	++this->furnaceItemStacks[2]->stackSize;
	}
	--this->furnaceItemStacks[0]->stackSize;
	if(this->furnaceItemStacks[0]->stackSize <= 0) {
	this->furnaceItemStacks[0] = NULL;
	}
	}
}

int TileEntityFurnace::getSmeltingResultItem(int var1)
{
	return var1 == Block::s_oreIron->blockID ? Item::s_ingotIron->shiftedIndex : (var1 == Block::s_oreGold->blockID ? Item::s_ingotGold->shiftedIndex : (var1 == Block::s_oreDiamond->blockID ? Item::s_diamond->shiftedIndex : (var1 == Block::s_sand->blockID ? Block::s_glass->blockID : (var1 == Item::s_porkRaw->shiftedIndex ? Item::s_porkCooked->shiftedIndex : (var1 == Item::s_fishRaw->shiftedIndex ? Item::s_fishCooked->shiftedIndex : (var1 == Block::s_cobblestone->blockID ? Block::s_stone->blockID : (var1 == Item::s_clay->shiftedIndex ? Item::s_brick->shiftedIndex : -1)))))));
}

int TileEntityFurnace::getItemBurnTime(ItemStack* var1)
{
	if(var1 == NULL) {
	return 0;
	} else {
	int var2 = var1->getItem()->shiftedIndex;
	return var2 < 256 && Block::s_blocksList[var2]->blockMaterial == Material::s_wood ? 300 : (var2 == Item::s_stick->shiftedIndex ? 100 : (var2 == Item::s_coal->shiftedIndex ? 1600 : (var2 == Item::s_bucketLava->shiftedIndex ? 20000 : 0)));
	}
}

void TileEntityFurnace::onInventoryChanged()
{
	TileEntity::onInventoryChanged();
}

void TileEntityFurnace::initStatics()
{
}
