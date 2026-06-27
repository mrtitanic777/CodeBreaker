#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from InventoryCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryCrafting.h"
#include "CraftingInventoryCB.h"
#include "IInventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

InventoryCrafting::InventoryCrafting(CraftingInventoryCB* var1, int var2, int var3)
	: nbrSlots(0)
	, eventHandler(NULL)
{
	this->nbrSlots = var2 * var3;
	this->stackList = JArray<ItemStack* >(this->nbrSlots);
	this->eventHandler = var1;
}

InventoryCrafting::InventoryCrafting(CraftingInventoryCB* var1, JArray<ItemStack* > var2)
	: nbrSlots(0)
	, eventHandler(NULL)
{
	this->nbrSlots = var2.length;
	this->stackList = var2;
	this->eventHandler = var1;
}

InventoryCrafting::~InventoryCrafting()
{
}

int InventoryCrafting::getSizeInventory()
{
	return this->nbrSlots;
}

ItemStack* InventoryCrafting::getStackInSlot(int var1)
{
	return this->stackList[var1];
}

JavaString InventoryCrafting::getInvName()
{
	return "Crafting";
}

ItemStack* InventoryCrafting::decrStackSize(int var1, int var2)
{
	if(this->stackList[var1] != NULL) {
	ItemStack* var3;
	if(this->stackList[var1]->stackSize <= var2) {
	var3 = this->stackList[var1];
	this->stackList[var1] = NULL;
	this->eventHandler->onCraftMatrixChanged(this);
	return var3;
	} else {
	var3 = this->stackList[var1]->splitStack(var2);
	if(this->stackList[var1]->stackSize == 0) {
	this->stackList[var1] = NULL;
	}
	this->eventHandler->onCraftMatrixChanged(this);
	return var3;
	}
	} else {
	return NULL;
	}
}

void InventoryCrafting::setInventorySlotContents(int var1, ItemStack* var2)
{
	this->stackList[var1] = var2;
	this->eventHandler->onCraftMatrixChanged(this);
}

int InventoryCrafting::getInventoryStackLimit()
{
	return 64;
}

void InventoryCrafting::onInventoryChanged()
{
}

void InventoryCrafting::initStatics()
{
}
