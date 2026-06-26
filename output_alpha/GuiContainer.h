#ifndef GUICONTAINER_H
#define GUICONTAINER_H

// Auto-generated from GuiContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <vector>

class RenderItem;
class Slot;
class SlotInventory;

class GuiContainer : public GuiScreen {
public:
	GuiContainer();
	virtual ~GuiContainer();

	virtual void drawScreen(int var1, int var2, float var3);
	virtual void onGuiClosed();
	virtual bool doesGuiPauseGame();

public: // was protected
	int m_xSize;
	int m_ySize;
	std::vector<void*> /* PORT: determine element type */ inventorySlots;

	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1) = 0;
	virtual void mouseClicked(int var1, int var2, int var3);
	virtual void mouseMovedOrUp(int var1, int var2, int var3);
	virtual void keyTyped(char var1, int var2);

public: // was private
	static RenderItem* s_itemRenderer;

	void drawSlotInventory(SlotInventory* var1);
	Slot* getSlotAtPosition(int var1, int var2);

};

#endif // GUICONTAINER_H