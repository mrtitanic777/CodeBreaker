#ifndef CRAFTINGINVENTORYPLAYERCB_H
#define CRAFTINGINVENTORYPLAYERCB_H

// Auto-generated from CraftingInventoryPlayerCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingInventoryCB.h"

class EntityPlayer;
class IInventory;
class InventoryCrafting;
class ItemStack;

class CraftingInventoryPlayerCB : public CraftingInventoryCB {
public:
	CraftingInventoryPlayerCB();
	CraftingInventoryPlayerCB(ItemStack** var1);
	virtual ~CraftingInventoryPlayerCB();

	InventoryCrafting* m_craftMatrix;
	IInventory* m_craftResult;

	virtual void onCraftMatrixChanged(IInventory* var1);
	virtual void onCraftGuiClosed(EntityPlayer* var1);

};

#endif // CRAFTINGINVENTORYPLAYERCB_H