#ifndef CRAFTINGINVENTORYCB_H
#define CRAFTINGINVENTORYCB_H

// Auto-generated from CraftingInventoryCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class EntityPlayer;
class IInventory;

class CraftingInventoryCB {
public:
	CraftingInventoryCB();
	virtual ~CraftingInventoryCB();

	JList<void*> unusedList;

	virtual void onCraftGuiClosed(EntityPlayer* var1);
	virtual void onCraftMatrixChanged(IInventory* var1);

	static void initStatics();
};

#endif // CRAFTINGINVENTORYCB_H