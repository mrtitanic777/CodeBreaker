#ifndef GUIINVENTORY_H
#define GUIINVENTORY_H

// Auto-generated from GuiInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"

class CraftingInventoryPlayerCB;
class IInventory;
class ItemStack;

class GuiInventory : public GuiContainer {
public:
	GuiInventory();
	GuiInventory(IInventory* var1, ItemStack** var2);
	virtual ~GuiInventory();

	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1);

public: // was private
	CraftingInventoryPlayerCB* m_field_977_j;
	float m_xSize_lo;
	float m_ySize_lo;

};

#endif // GUIINVENTORY_H