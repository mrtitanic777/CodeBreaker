#ifndef GUICONTAINER_H
#define GUICONTAINER_H

// Auto-generated from GuiContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include "ItemStack.h"
#include <vector>

class RenderItem;

class GuiContainer : public GuiScreen {
public:
	GuiContainer();
	virtual ~GuiContainer();

	virtual void drawScreen(int var1, int var2, float var3);
	virtual void onGuiClosed();
	virtual void guiCraftingItemsCheck();
	virtual bool doesGuiPauseGame();

protected:
	int m_xSize;
	int m_ySize;
	std::vector<void*> /* PORT: determine element type */ inventorySlots;

	virtual void drawGuiContainerForegroundLayer();
	virtual void drawGuiContainerBackgroundLayer() = 0;
	virtual void drawSlotInventory(int var1, int var2, int var3);
	virtual void keyTyped(char var1, int var2);

private:
	static RenderItem* s_itemRenderer;
	ItemStack m_itemStack;
	bool m_hasItemStack;

};

#endif // GUICONTAINER_H