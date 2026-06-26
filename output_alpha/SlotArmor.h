#ifndef SLOTARMOR_H
#define SLOTARMOR_H

// Auto-generated from SlotArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SlotInventory.h"

class GuiContainer;
class GuiInventory;
class IInventory;
class ItemStack;

class SlotArmor : public SlotInventory {
public:
	SlotArmor();
	SlotArmor(GuiInventory* var1, GuiContainer* var2, IInventory* var3, int var4, int var5, int var6, int var7);
	virtual ~SlotArmor();

	virtual int getSlotStackLimit();
	virtual bool isItemValid(ItemStack* var1);
	virtual int func_775_c();

public: // was private
	int m_field_1124_c;
	GuiInventory* m_field_1123_d;


};

#endif // SLOTARMOR_H