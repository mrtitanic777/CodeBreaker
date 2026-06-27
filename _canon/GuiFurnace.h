#ifndef GUIFURNACE_H
#define GUIFURNACE_H

// Auto-generated from GuiFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiContainer.h"

class InventoryPlayer;
class TileEntityFurnace;

class GuiFurnace : public GuiContainer {
public:
	GuiFurnace(InventoryPlayer* var1, TileEntityFurnace* var2);
	virtual ~GuiFurnace();

	TileEntityFurnace* furnace;

	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1);

	static void initStatics();
};

#endif // GUIFURNACE_H