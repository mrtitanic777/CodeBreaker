#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Slot.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Slot.h"
#include "IInventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Random.h"

Slot::Slot(IInventory* var1, int var2)
	: slotIndex(0)
	, inventory(NULL)
{
	this->inventory = var1;
	this->slotIndex = var2;
}

Slot::~Slot()
{
}

void Slot::onPickupFromSlot()
{
	this->onSlotChanged();
}

bool Slot::isItemValid(ItemStack* var1)
{
	return true;
}

ItemStack* Slot::getStack()
{
	return this->inventory->getStackInSlot(this->slotIndex);
}

void Slot::putStack(ItemStack* var1)
{
	this->inventory->setInventorySlotContents(this->slotIndex, var1);
	this->onSlotChanged();
}

int Slot::getBackgroundIconIndex()
{
	return -1;
}

void Slot::onSlotChanged()
{
	this->inventory->onInventoryChanged();
}

int Slot::getSlotStackLimit()
{
	return this->inventory->getInventoryStackLimit();
}

void Slot::initStatics()
{
}
