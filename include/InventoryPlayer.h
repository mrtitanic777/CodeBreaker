#ifndef INVENTORYPLAYER_H
#define INVENTORYPLAYER_H

// Auto-generated from InventoryPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include "ItemStack.h"
#include <string>

class InventoryPlayer : public IInventory {
public:
	InventoryPlayer();
	virtual ~InventoryPlayer();

	ItemStack* m_mainInventory;
	int m_currentItem;

	virtual ItemStack getCurrentItem();
	virtual void getFirstEmptyStack(int var1);
	virtual bool consumeInventoryItem(int var1);
	virtual bool storePartialItemStack(const ItemStack& var1);
	virtual ItemStack decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, const ItemStack& var2);
	virtual int getSizeInventory();
	virtual ItemStack getStackInSlot(int var1);
	virtual const std::string& getInvName();
	virtual int getInventoryStackLimit();

private:
	int getInventorySlotContainItem(int var1);
	int storeItemStack();

};

#endif // INVENTORYPLAYER_H