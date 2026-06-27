#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from InventoryLargeChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryLargeChest.h"
#include "IInventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

InventoryLargeChest::InventoryLargeChest(const JavaString& var1, IInventory* var2, IInventory* var3)
	: upperChest(NULL)
	, lowerChest(NULL)
{
	this->name = var1;
	this->upperChest = var2;
	this->lowerChest = var3;
}

InventoryLargeChest::~InventoryLargeChest()
{
}

int InventoryLargeChest::getSizeInventory()
{
	return this->upperChest->getSizeInventory() + this->lowerChest->getSizeInventory();
}

JavaString InventoryLargeChest::getInvName()
{
	return this->name;
}

ItemStack* InventoryLargeChest::getStackInSlot(int var1)
{
	return var1 >= this->upperChest->getSizeInventory() ? this->lowerChest->getStackInSlot(var1 - this->upperChest->getSizeInventory()) : this->upperChest->getStackInSlot(var1);
}

ItemStack* InventoryLargeChest::decrStackSize(int var1, int var2)
{
	return var1 >= this->upperChest->getSizeInventory() ? this->lowerChest->decrStackSize(var1 - this->upperChest->getSizeInventory(), var2) : this->upperChest->decrStackSize(var1, var2);
}

void InventoryLargeChest::setInventorySlotContents(int var1, ItemStack* var2)
{
	if(var1 >= this->upperChest->getSizeInventory()) {
	this->lowerChest->setInventorySlotContents(var1 - this->upperChest->getSizeInventory(), var2);
	} else {
	this->upperChest->setInventorySlotContents(var1, var2);
	}
}

int InventoryLargeChest::getInventoryStackLimit()
{
	return this->upperChest->getInventoryStackLimit();
}

void InventoryLargeChest::onInventoryChanged()
{
	this->upperChest->onInventoryChanged();
	this->lowerChest->onInventoryChanged();
}

void InventoryLargeChest::initStatics()
{
}
