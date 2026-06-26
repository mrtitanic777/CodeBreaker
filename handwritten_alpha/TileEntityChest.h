#ifndef TILEENTITYCHEST_H
#define TILEENTITYCHEST_H

// Auto-generated from TileEntityChest.java  (handwritten_alpha override:
// adds the onInventoryChanged() override so the class is concrete — see .cpp)
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include "TileEntity.h"
#include <string>

class ItemStack;
class NBTTagCompound;

class TileEntityChest : public TileEntity, public IInventory {
public:
	TileEntityChest();
	virtual ~TileEntityChest();

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual const std::string& getInvName();
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual void writeToNBT(NBTTagCompound* var1);
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();

public: // was private
	ItemStack** m_field_827_a;

public:  // array-length companions
	int m_field_827_a_count;

};

#endif // TILEENTITYCHEST_H
