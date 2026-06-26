#ifndef GUICRAFTING_H
#define GUICRAFTING_H

// Auto-generated from GuiCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"

class CraftingInventoryWorkbenchCB;
class InventoryPlayer;

class GuiCrafting : public GuiContainer {
public:
	GuiCrafting();
	GuiCrafting(InventoryPlayer* var1);
	virtual ~GuiCrafting();

	CraftingInventoryWorkbenchCB* m_field_979_j;

	virtual void onGuiClosed();

public: // was protected
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1);

};

#endif // GUICRAFTING_H