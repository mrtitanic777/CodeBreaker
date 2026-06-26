#ifndef GUIFURNACE_H
#define GUIFURNACE_H

// Auto-generated from GuiFurnace.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"

class InventoryPlayer;
class TileEntityFurnace;

class GuiFurnace : public GuiContainer {
public:
	GuiFurnace();
	GuiFurnace(InventoryPlayer* var1, TileEntityFurnace* var2);
	virtual ~GuiFurnace();


public: // was protected
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1);

public: // was private
	TileEntityFurnace* m_field_978_j;

};

#endif // GUIFURNACE_H