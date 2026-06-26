#ifndef IINVENTORY_H
#define IINVENTORY_H

// Auto-generated from IInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class ItemStack;

class IInventory {
public:
	virtual ~IInventory() {}
	virtual int getSizeInventory() = 0;
	virtual ItemStack* getStackInSlot(int var1) = 0;
	virtual ItemStack* decrStackSize(int var1, int var2) = 0;
	virtual void setInventorySlotContents(int var1, ItemStack* var2) = 0;
	virtual const std::string& getInvName() = 0;
	virtual int getInventoryStackLimit() = 0;
	virtual void onInventoryChanged() = 0;
};

#endif // IINVENTORY_H