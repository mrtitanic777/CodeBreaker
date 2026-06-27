#ifndef SLOTARMOR_H
#define SLOTARMOR_H

// Auto-generated from SlotArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "SlotInventory.h"

class GuiContainer;
class GuiInventory;
class IInventory;
class ItemStack;

class SlotArmor : public SlotInventory {
public:
	SlotArmor(GuiInventory* var1, GuiContainer* var2, IInventory* var3, int var4, int var5, int var6, int var7);
	virtual ~SlotArmor();

	int armorType;
	GuiInventory* guiInventory;

	virtual int getSlotStackLimit();
	virtual bool isItemValid(ItemStack* var1);
	virtual int getBackgroundIconIndex();

	static void initStatics();
};

#endif // SLOTARMOR_H