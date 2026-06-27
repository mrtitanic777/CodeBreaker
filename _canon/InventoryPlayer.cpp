#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from InventoryPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryPlayer.h"
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
#include "Item.h"
#include "ItemArmor.h"
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

InventoryPlayer::InventoryPlayer(EntityPlayer* var1)
	: currentItem(0)
	, player(NULL)
	, draggingItemStack(NULL)
	, inventoryChanged(0)
{
	this->mainInventory = JArray<ItemStack* >(37);
	this->armorInventory = JArray<ItemStack* >(4);
	this->craftingInventory = JArray<ItemStack* >(4);
	this->currentItem = 0;
	this->inventoryChanged = false;
	this->player = var1;
}

InventoryPlayer::~InventoryPlayer()
{
}

ItemStack* InventoryPlayer::getCurrentItem()
{
	return this->mainInventory[this->currentItem];
}

int InventoryPlayer::getInventorySlotContainItem(int var1)
{
	for(int var2 = 0; var2 < this->mainInventory.length; ++var2) {
	if(this->mainInventory[var2] != NULL && this->mainInventory[var2]->itemID == var1) {
	return var2;
	}
	}
	return -1;
}

int InventoryPlayer::getFirstPartialMatchingStack(int var1)
{
	for(int var2 = 0; var2 < this->mainInventory.length; ++var2) {
	if(this->mainInventory[var2] != NULL && this->mainInventory[var2]->itemID == var1 && this->mainInventory[var2]->stackSize < this->mainInventory[var2]->getMaxStackSize() && this->mainInventory[var2]->stackSize < this->getInventoryStackLimit()) {
	return var2;
	}
	}
	return -1;
}

int InventoryPlayer::getFirstEmptyStack()
{
	for(int var1 = 0; var1 < this->mainInventory.length; ++var1) {
	if(this->mainInventory[var1] == NULL) {
	return var1;
	}
	}
	return -1;
}

void InventoryPlayer::setCurrentItem(int var1, bool var2)
{
	int var3 = this->getInventorySlotContainItem(var1);
	if(var3 >= 0 && var3 < 9) {
	this->currentItem = var3;
	}
}

void InventoryPlayer::changeCurrentItem(int var1)
{
	if(var1 > 0) {
	var1 = 1;
	}
	if(var1 < 0) {
	var1 = -1;
	}
	for(this->currentItem -= var1; this->currentItem < 0; this->currentItem += 9) {
	}
	while(this->currentItem >= 9) {
	this->currentItem -= 9;
	}
}

int InventoryPlayer::addItemsToInventory(int var1, int var2)
{
	int var3 = this->getFirstPartialMatchingStack(var1);
	if(var3 < 0) {
	var3 = this->getFirstEmptyStack();
	}
	if(var3 < 0) {
	return var2;
	} else {
	if(this->mainInventory[var3] == NULL) {
	this->mainInventory[var3] = new ItemStack(var1, 0);
	}
	int var4 = var2;
	if(var2 > this->mainInventory[var3]->getMaxStackSize() - this->mainInventory[var3]->stackSize) {
	var4 = this->mainInventory[var3]->getMaxStackSize() - this->mainInventory[var3]->stackSize;
	}
	if(var4 > this->getInventoryStackLimit() - this->mainInventory[var3]->stackSize) {
	var4 = this->getInventoryStackLimit() - this->mainInventory[var3]->stackSize;
	}
	if(var4 == 0) {
	return var2;
	} else {
	var2 -= var4;
	this->mainInventory[var3]->stackSize += var4;
	this->mainInventory[var3]->animationsToGo = 5;
	return var2;
	}
	}
}

void InventoryPlayer::decrementAnimations()
{
	for(int var1 = 0; var1 < this->mainInventory.length; ++var1) {
	if(this->mainInventory[var1] != NULL && this->mainInventory[var1]->animationsToGo > 0) {
	--this->mainInventory[var1]->animationsToGo;
	}
	}
}

bool InventoryPlayer::consumeInventoryItem(int var1)
{
	int var2 = this->getInventorySlotContainItem(var1);
	if(var2 < 0) {
	return false;
	} else {
	if(--this->mainInventory[var2]->stackSize <= 0) {
	this->mainInventory[var2] = NULL;
	}
	return true;
	}
}

bool InventoryPlayer::addItemStackToInventory(ItemStack* var1)
{
	if(var1->itemDamage == 0) {
	var1->stackSize = this->addItemsToInventory(var1->itemID, var1->stackSize);
	if(var1->stackSize == 0) {
	return true;
	}
	}
	int var2 = this->getFirstEmptyStack();
	if(var2 >= 0) {
	this->mainInventory[var2] = var1;
	this->mainInventory[var2]->animationsToGo = 5;
	return true;
	} else {
	return false;
	}
}

ItemStack* InventoryPlayer::decrStackSize(int var1, int var2)
{
	JArray<ItemStack* > var3 = this->mainInventory;
	if(var1 >= this->mainInventory.length) {
	var3 = this->armorInventory;
	var1 -= this->mainInventory.length;
	}
	if(var3[var1] != NULL) {
	ItemStack* var4;
	if(var3[var1]->stackSize <= var2) {
	var4 = var3[var1];
	var3[var1] = NULL;
	return var4;
	} else {
	var4 = var3[var1]->splitStack(var2);
	if(var3[var1]->stackSize == 0) {
	var3[var1] = NULL;
	}
	return var4;
	}
	} else {
	return NULL;
	}
}

void InventoryPlayer::setInventorySlotContents(int var1, ItemStack* var2)
{
	JArray<ItemStack* > var3 = this->mainInventory;
	if(var1 >= var3.length) {
	var1 -= var3.length;
	var3 = this->armorInventory;
	}
	if(var1 >= var3.length) {
	var1 -= var3.length;
	var3 = this->craftingInventory;
	}
	var3[var1] = var2;
}

float InventoryPlayer::getStrVsBlock(Block* var1)
{
	float var2 = 1.0F;
	if(this->mainInventory[this->currentItem] != NULL) {
	var2 *= this->mainInventory[this->currentItem]->getStrVsBlock(var1);
	}
	return var2;
}

NBTTagList* InventoryPlayer::writeToNBT(NBTTagList* var1)
{
	int var2;
	NBTTagCompound* var3;
	for(var2 = 0; var2 < this->mainInventory.length; ++var2) {
	if(this->mainInventory[var2] != NULL) {
	var3 = new NBTTagCompound();
	var3->setByte("Slot", (unsigned char)var2);
	if (this->mainInventory[var2] != NULL) { this->mainInventory[var2]->writeToNBT(var3); }
	var1->setTag(var3);
	}
	}
	for(var2 = 0; var2 < this->armorInventory.length; ++var2) {
	if(this->armorInventory[var2] != NULL) {
	var3 = new NBTTagCompound();
	var3->setByte("Slot",(unsigned char)(var2 + 100));
	if (this->armorInventory[var2] != NULL) { this->armorInventory[var2]->writeToNBT(var3); }
	var1->setTag(var3);
	}
	}
	for(var2 = 0; var2 < this->craftingInventory.length; ++var2) {
	if(this->craftingInventory[var2] != NULL) {
	var3 = new NBTTagCompound();
	var3->setByte("Slot",(unsigned char)(var2 + 80));
	if (this->craftingInventory[var2] != NULL) { this->craftingInventory[var2]->writeToNBT(var3); }
	var1->setTag(var3);
	}
	}
	return var1;
}

void InventoryPlayer::readFromNBT(NBTTagList* var1)
{
	this->mainInventory = JArray<ItemStack* >(36);
	this->armorInventory = JArray<ItemStack* >(4);
	this->craftingInventory = JArray<ItemStack* >(4);
	for(int var2 = 0; var2 < var1->tagCount(); ++var2) {
	NBTTagCompound* var3 = (NBTTagCompound*)var1->tagAt(var2);
	int var4 = var3->getByte("Slot") & 255;
	if(var4 >= 0 && var4 < this->mainInventory.length) {
	this->mainInventory[var4] = new ItemStack(var3);
	}
	if(var4 >= 80 && var4 < this->craftingInventory.length + 80) {
	this->craftingInventory[var4 - 80] = new ItemStack(var3);
	}
	if(var4 >= 100 && var4 < this->armorInventory.length + 100) {
	this->armorInventory[var4 - 100] = new ItemStack(var3);
	}
	}
}

int InventoryPlayer::getSizeInventory()
{
	return this->mainInventory.length + 4;
}

ItemStack* InventoryPlayer::getStackInSlot(int var1)
{
	JArray<ItemStack* > var2 = this->mainInventory;
	if(var1 >= var2.length) {
	var1 -= var2.length;
	var2 = this->armorInventory;
	}
	if(var1 >= var2.length) {
	var1 -= var2.length;
	var2 = this->craftingInventory;
	}
	return var2[var1];
}

JavaString InventoryPlayer::getInvName()
{
	return "Inventory";
}

int InventoryPlayer::getInventoryStackLimit()
{
	return 64;
}

int InventoryPlayer::getDamageVsEntity(Entity* var1)
{
	ItemStack* var2 = this->getStackInSlot(this->currentItem);
	return var2 != NULL ? var2->getDamageVsEntity(var1) : 1;
}

bool InventoryPlayer::canHarvestBlock(Block* var1)
{
	if(var1->blockMaterial != Material::s_rock && var1->blockMaterial != Material::s_iron && var1->blockMaterial != Material::s_builtSnow && var1->blockMaterial != Material::s_snow) {
	return true;
	} else {
	ItemStack* var2 = this->getStackInSlot(this->currentItem);
	return var2 != NULL ? var2->canHarvestBlock(var1) : false;
	}
}

ItemStack* InventoryPlayer::armorItemInSlot(int var1)
{
	return this->armorInventory[var1];
}

int InventoryPlayer::getTotalArmorValue()
{
	int var1 = 0;
	int var2 = 0;
	int var3 = 0;
	for(int var4 = 0; var4 < this->armorInventory.length; ++var4) {
	if(this->armorInventory[var4] != NULL && (dynamic_cast<ItemArmor*>(this->armorInventory[var4]->getItem()) != NULL)) {
	int var5 = this->armorInventory[var4]->getMaxDamage();
	int var6 = this->armorInventory[var4]->itemDamage;
	int var7 = var5 - var6;
	var2 += var7;
	var3 += var5;
	int var8 = ((ItemArmor*)this->armorInventory[var4]->getItem())->damageReduceAmmount;
	var1 += var8;
	}
	}
	if(var3 == 0) {
	return 0;
	} else {
	return (var1 - 1) * var2 / var3 + 1;
	}
}

void InventoryPlayer::damageArmor(int var1)
{
	for(int var2 = 0; var2 < this->armorInventory.length; ++var2) {
	if(this->armorInventory[var2] != NULL && (dynamic_cast<ItemArmor*>(this->armorInventory[var2]->getItem()) != NULL)) {
	if (this->armorInventory[var2] != NULL) { this->armorInventory[var2]->damageItem(var1); }
	if(this->armorInventory[var2]->stackSize == 0) {
	if (this->armorInventory[var2] != NULL) { this->armorInventory[var2]->onItemDestroyedByUse(this->player); }
	this->armorInventory[var2] = NULL;
	}
	}
	}
}

void InventoryPlayer::dropAllItems()
{
	int var1;
	for(var1 = 0; var1 < this->mainInventory.length; ++var1) {
	if(this->mainInventory[var1] != NULL) {
	this->player->dropPlayerItemWithRandomChoice(this->mainInventory[var1], true);
	this->mainInventory[var1] = NULL;
	}
	}
	for(var1 = 0; var1 < this->armorInventory.length; ++var1) {
	if(this->armorInventory[var1] != NULL) {
	this->player->dropPlayerItemWithRandomChoice(this->armorInventory[var1], true);
	this->armorInventory[var1] = NULL;
	}
	}
}

void InventoryPlayer::onInventoryChanged()
{
	this->inventoryChanged = true;
}

bool InventoryPlayer::compareInventory(InventoryPlayer* var1)
{
	int var2;
	for(var2 = 0; var2 < this->mainInventory.length; ++var2) {
	if(!this->compareItemStack(var1->mainInventory[var2], this->mainInventory[var2])) {
	return false;
	}
	}
	for(var2 = 0; var2 < this->armorInventory.length; ++var2) {
	if(!this->compareItemStack(var1->armorInventory[var2], this->armorInventory[var2])) {
	return false;
	}
	}
	for(var2 = 0; var2 < this->craftingInventory.length; ++var2) {
	if(!this->compareItemStack(var1->craftingInventory[var2], this->craftingInventory[var2])) {
	return false;
	}
	}
	return true;
}

bool InventoryPlayer::compareItemStack(ItemStack* var1, ItemStack* var2)
{
	return var1 == NULL && var2 == NULL ? true : (var1 != NULL && var2 != NULL ? var1->itemID == var2->itemID && var1->stackSize == var2->stackSize && var1->itemDamage == var2->itemDamage : false);
}

InventoryPlayer* InventoryPlayer::copyInventory()
{
	InventoryPlayer* var1 = new InventoryPlayer((EntityPlayer*)NULL);
	int var2;
	for(var2 = 0; var2 < this->mainInventory.length; ++var2) {
	var1->mainInventory[var2] = this->mainInventory[var2] != NULL ? this->mainInventory[var2]->copy() : NULL;
	}
	for(var2 = 0; var2 < this->armorInventory.length; ++var2) {
	var1->armorInventory[var2] = this->armorInventory[var2] != NULL ? this->armorInventory[var2]->copy() : NULL;
	}
	for(var2 = 0; var2 < this->craftingInventory.length; ++var2) {
	var1->craftingInventory[var2] = this->craftingInventory[var2] != NULL ? this->craftingInventory[var2]->copy() : NULL;
	}
	return var1;
}

void InventoryPlayer::initStatics()
{
}
