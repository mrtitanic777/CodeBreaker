#ifndef GUICHEST_H
#define GUICHEST_H

// Auto-generated from GuiChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiContainer.h"

class IInventory;

class GuiChest : public GuiContainer {
public:
	GuiChest(IInventory* var1, IInventory* var2);
	virtual ~GuiChest();

	IInventory* lowerPlayerInventory;
	IInventory* upperChestInventory;
	int numRows;

	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1);

	static void initStatics();
};

#endif // GUICHEST_H