#ifndef GUICRAFTING_H
#define GUICRAFTING_H

// Auto-generated from GuiCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiContainer.h"

class CraftingInventoryWorkbenchCB;
class InventoryPlayer;

class GuiCrafting : public GuiContainer {
public:
	GuiCrafting(InventoryPlayer* var1);
	virtual ~GuiCrafting();

	CraftingInventoryWorkbenchCB* craftingInventory;

	virtual void onGuiClosed();
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1);

	static void initStatics();
};

#endif // GUICRAFTING_H