#ifndef TILEENTITYFURNACE_H
#define TILEENTITYFURNACE_H

// Auto-generated from TileEntityFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9
// HANDWRITTEN OVERRIDE: declares onInventoryChanged() so the class is concrete.
//  onInventoryChanged() is pure-virtual in IInventory; without an explicit
//  declaration here MSVC does not treat TileEntity's same-named method as the
//  final overrider across the two bases, leaving TileEntityFurnace abstract
//  (which broke "new TileEntityFurnace()" in BlockFurnace). See TileEntityChest.h.

#include "IInventory.h"
#include "TileEntity.h"
#include <string>

class ItemStack;
class NBTTagCompound;

class TileEntityFurnace : public TileEntity, public IInventory {
public:
	TileEntityFurnace();
	virtual ~TileEntityFurnace();

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual const std::string& getInvName();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual int getInventoryStackLimit();
	virtual int getCookProgressScaled(int var1);
	virtual int getBurnTimeRemainingScaled(int var1);
	virtual bool isBurning();
	virtual void updateEntity();
	virtual void smeltItem();
	virtual void onInventoryChanged();  // PORT: satisfies IInventory pure-virtual

public: // was private
	ItemStack** m_furnaceItemStacks;
	int m_furnaceBurnTime;
	int m_currentItemBurnTime;
	int m_furnaceCookTime;

	bool canSmelt();
	int getSmeltingResultItem(int var1);
	int getItemBurnTime(ItemStack* var1);

public:  // array-length companions
	int m_furnaceItemStacks_count;

};

#endif // TILEENTITYFURNACE_H
