#ifndef GUICHEST_H
#define GUICHEST_H

// Auto-generated from GuiChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"

class IInventory;

class GuiChest : public GuiContainer {
public:
	GuiChest();
	GuiChest(IInventory* var1, IInventory* var2);
	virtual ~GuiChest();


public: // was protected
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer();

public: // was private
	IInventory* m_upperChestInventory;
	IInventory* m_lowerChestInventory;
	int m_inventoryRows;

};

#endif // GUICHEST_H