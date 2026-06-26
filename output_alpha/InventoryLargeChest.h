#ifndef INVENTORYLARGECHEST_H
#define INVENTORYLARGECHEST_H

// Auto-generated from InventoryLargeChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include <string>

class ItemStack;

class InventoryLargeChest : public IInventory {
public:
	InventoryLargeChest();
	InventoryLargeChest(const std::string& var1, IInventory* var2, IInventory* var3);
	virtual ~InventoryLargeChest();

	virtual int getSizeInventory();
	virtual const std::string& getInvName();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();

public: // was private
	std::string m_name;
	IInventory* m_upperChest;
	IInventory* m_lowerChest;

};

#endif // INVENTORYLARGECHEST_H