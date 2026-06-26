#ifndef GUICRAFTING_H
#define GUICRAFTING_H

// Auto-generated from GuiCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"

class IInventory;
class InventoryCrafting;
class InventoryPlayer;

class GuiCrafting : public GuiContainer {
public:
	GuiCrafting();
	GuiCrafting(InventoryPlayer* var1);
	virtual ~GuiCrafting();

	virtual void onGuiClosed();
	virtual void guiCraftingItemsCheck();

public: // was protected
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer();

public: // was private
	InventoryCrafting* m_inventoryCrafting;
	IInventory* m_iInventory;

};

#endif // GUICRAFTING_H