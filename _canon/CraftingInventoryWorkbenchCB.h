#ifndef CRAFTINGINVENTORYWORKBENCHCB_H
#define CRAFTINGINVENTORYWORKBENCHCB_H

// Auto-generated from CraftingInventoryWorkbenchCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "CraftingInventoryCB.h"

class EntityPlayer;
class IInventory;
class InventoryCrafting;

class CraftingInventoryWorkbenchCB : public CraftingInventoryCB {
public:
	CraftingInventoryWorkbenchCB();
	virtual ~CraftingInventoryWorkbenchCB();

	InventoryCrafting* craftMatrix;
	IInventory* craftResult;

	virtual void onCraftMatrixChanged(IInventory* var1);
	virtual void onCraftGuiClosed(EntityPlayer* var1);

	static void initStatics();
};

#endif // CRAFTINGINVENTORYWORKBENCHCB_H