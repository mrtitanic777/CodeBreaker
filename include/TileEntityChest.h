#ifndef TILEENTITYCHEST_H
#define TILEENTITYCHEST_H

// Auto-generated from TileEntityChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include "ItemStack.h"
#include "TileEntity.h"
#include <string>

class NBTTagCompound;

class TileEntityChest : public TileEntity, public IInventory {
public:
	TileEntityChest();
	virtual ~TileEntityChest();

	virtual int getSizeInventory();
	virtual ItemStack getStackInSlot(int var1);
	virtual ItemStack decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, const ItemStack& var2);
	virtual const std::string& getInvName();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual int getInventoryStackLimit();

private:
	ItemStack* m_chestContents;

};

#endif // TILEENTITYCHEST_H