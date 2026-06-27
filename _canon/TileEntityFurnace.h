#ifndef TILEENTITYFURNACE_H
#define TILEENTITYFURNACE_H

// Auto-generated from TileEntityFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IInventory.h"
#include "TileEntity.h"

class ItemStack;
class NBTTagCompound;

class TileEntityFurnace : public TileEntity, public IInventory {
public:
	TileEntityFurnace();
	virtual ~TileEntityFurnace();

	JArray<ItemStack* > furnaceItemStacks;
	int furnaceBurnTime;
	int currentItemBurnTime;
	int furnaceCookTime;

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual JavaString getInvName();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual int getInventoryStackLimit();
	virtual int getCookProgressScaled(int var1);
	virtual int getBurnTimeRemainingScaled(int var1);
	virtual bool isBurning();
	virtual void updateEntity();
	virtual bool canSmelt();
	virtual void smeltItem();
	virtual int getSmeltingResultItem(int var1);
	virtual int getItemBurnTime(ItemStack* var1);

	static void initStatics();

	virtual void onInventoryChanged();
};

#endif // TILEENTITYFURNACE_H