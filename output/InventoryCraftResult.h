#ifndef INVENTORYCRAFTRESULT_H
#define INVENTORYCRAFTRESULT_H

// Auto-generated from InventoryCraftResult.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include <string>

class ItemStack;

class InventoryCraftResult : public IInventory {
public:
	InventoryCraftResult();
	virtual ~InventoryCraftResult();

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual const std::string& getInvName();
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual int getInventoryStackLimit();

public: // was private
	ItemStack** m_stackResult;

public:  // array-length companions
	int m_stackResult_count;

};

#endif // INVENTORYCRAFTRESULT_H