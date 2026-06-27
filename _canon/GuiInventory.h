#ifndef GUIINVENTORY_H
#define GUIINVENTORY_H

// Auto-generated from GuiInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiContainer.h"

class CraftingInventoryPlayerCB;
class IInventory;
class ItemStack;

class GuiInventory : public GuiContainer {
public:
	GuiInventory(IInventory* var1, JArray<ItemStack* > var2);
	virtual ~GuiInventory();

	CraftingInventoryPlayerCB* craftingInventory;
	float xSize_lo;
	float ySize_lo;

	virtual void drawGuiContainerForegroundLayer();
	virtual void drawScreen(int var1, int var2, float var3);
	virtual void drawGuiContainerBackgroundLayer(float var1);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	GuiInventory(const GuiInventory&);
	GuiInventory& operator=(const GuiInventory&);
};

#endif // GUIINVENTORY_H