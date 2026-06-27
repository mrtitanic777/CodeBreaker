#ifndef CRAFTINGINVENTORYPLAYERCB_H
#define CRAFTINGINVENTORYPLAYERCB_H

// Auto-generated from CraftingInventoryPlayerCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "CraftingInventoryCB.h"

class EntityPlayer;
class IInventory;
class InventoryCrafting;
class ItemStack;

class CraftingInventoryPlayerCB : public CraftingInventoryCB {
public:
	CraftingInventoryPlayerCB(JArray<ItemStack* > var1);
	virtual ~CraftingInventoryPlayerCB();

	InventoryCrafting* craftMatrix;
	IInventory* craftResult;

	virtual void onCraftMatrixChanged(IInventory* var1);
	virtual void onCraftGuiClosed(EntityPlayer* var1);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	CraftingInventoryPlayerCB(const CraftingInventoryPlayerCB&);
	CraftingInventoryPlayerCB& operator=(const CraftingInventoryPlayerCB&);
};

#endif // CRAFTINGINVENTORYPLAYERCB_H