#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from InventoryCraftResult.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryCraftResult.h"
#include "IInventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

InventoryCraftResult::InventoryCraftResult()
{
	this->stackResult = JArray<ItemStack* >(1);
}

InventoryCraftResult::~InventoryCraftResult()
{
}

int InventoryCraftResult::getSizeInventory()
{
	return 1;
}

ItemStack* InventoryCraftResult::getStackInSlot(int var1)
{
	return this->stackResult[var1];
}

JavaString InventoryCraftResult::getInvName()
{
	return "Result";
}

ItemStack* InventoryCraftResult::decrStackSize(int var1, int var2)
{
	if(this->stackResult[var1] != NULL) {
	ItemStack* var3 = this->stackResult[var1];
	this->stackResult[var1] = NULL;
	return var3;
	} else {
	return NULL;
	}
}

void InventoryCraftResult::setInventorySlotContents(int var1, ItemStack* var2)
{
	this->stackResult[var1] = var2;
}

int InventoryCraftResult::getInventoryStackLimit()
{
	return 64;
}

void InventoryCraftResult::onInventoryChanged()
{
}

void InventoryCraftResult::initStatics()
{
}
