#ifndef GUIINVENTORY_H
#define GUIINVENTORY_H

// Auto-generated from GuiInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"

class IInventory;
class InventoryCrafting;

class GuiInventory : public GuiContainer {
public:
	GuiInventory();
	GuiInventory(IInventory* var1);
	virtual ~GuiInventory();

	virtual void onGuiClosed();
	virtual void guiCraftingItemsCheck();
	virtual void drawScreen(int var1, int var2, float var3);

protected:
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer();

private:
	InventoryCrafting* m_inventoryCrafting;
	IInventory* m_iInventory;
	float m_xSize_lo;
	float m_ySize_lo;

};

#endif // GUIINVENTORY_H