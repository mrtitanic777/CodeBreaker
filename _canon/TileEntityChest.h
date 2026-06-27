#ifndef TILEENTITYCHEST_H
#define TILEENTITYCHEST_H

// Auto-generated from TileEntityChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IInventory.h"
#include "TileEntity.h"

class ItemStack;
class NBTTagCompound;

class TileEntityChest : public TileEntity, public IInventory {
public:
	TileEntityChest();
	virtual ~TileEntityChest();

	JArray<ItemStack* > chestContents;

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual JavaString getInvName();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual int getInventoryStackLimit();

	static void initStatics();

	virtual void onInventoryChanged();
};

#endif // TILEENTITYCHEST_H