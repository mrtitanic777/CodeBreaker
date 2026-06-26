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
	virtual void drawGuiContainerBackgroundLayer(float var1);

public: // was private
	IInventory* m_field_982_j;
	IInventory* m_field_981_l;
	int m_field_980_m;

};

#endif // GUICHEST_H