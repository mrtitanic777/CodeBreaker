#ifndef GUICONTAINER_H
#define GUICONTAINER_H

// Auto-generated from GuiContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class RenderItem;
class Slot;
class SlotInventory;

class GuiContainer : public GuiScreen {
public:
	GuiContainer();
	virtual ~GuiContainer();

	int xSize;
	int ySize;
	JList<void*> inventorySlots;
	static RenderItem* s_itemRenderer;

	virtual void drawScreen(int var1, int var2, float var3);
	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer(float var1) = 0;
	virtual void drawSlotInventory(SlotInventory* var1);
	virtual Slot* getSlotAtPosition(int var1, int var2);
	virtual void mouseClicked(int var1, int var2, int var3);
	virtual void mouseMovedOrUp(int var1, int var2, int var3);
	virtual void keyTyped(char var1, int var2);
	virtual void onGuiClosed();
	virtual bool doesGuiPauseGame();

	static void initStatics();
};

#endif // GUICONTAINER_H