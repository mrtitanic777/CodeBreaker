#ifndef CRAFTINGINVENTORYWORKBENCHCB_H
#define CRAFTINGINVENTORYWORKBENCHCB_H

// Auto-generated from CraftingInventoryWorkbenchCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingInventoryCB.h"

class EntityPlayer;
class IInventory;
class InventoryCrafting;

class CraftingInventoryWorkbenchCB : public CraftingInventoryCB {
public:
	CraftingInventoryWorkbenchCB();
	virtual ~CraftingInventoryWorkbenchCB();

	InventoryCrafting* m_craftMatrix;
	IInventory* m_craftResult;

	virtual void onCraftMatrixChanged(IInventory* var1);
	virtual void onCraftGuiClosed(EntityPlayer* var1);

};

#endif // CRAFTINGINVENTORYWORKBENCHCB_H