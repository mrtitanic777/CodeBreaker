#ifndef SLOT_H
#define SLOT_H

// Auto-generated from Slot.java
// Target: Xbox 360 / C++03 / DirectX 9

class GuiContainer;
class IInventory;
class ItemStack;

class Slot {
public:
	Slot();
	Slot(GuiContainer* var1, IInventory* var2, int var3, int var4, int var5);
	virtual ~Slot();

	int m_slotIndex;
	int m_xPos;
	int m_yPos;
	IInventory* m_inventory;

	virtual bool isAtCursorPos(int var1, int var2);
	virtual void onPickupFromSlot();
	virtual bool isItemValid();
	virtual void putStack(const ItemStack& var1);

private:
	GuiContainer* m_guiHandler;

};

#endif // SLOT_H