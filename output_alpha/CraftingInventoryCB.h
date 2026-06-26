#ifndef CRAFTINGINVENTORYCB_H
#define CRAFTINGINVENTORYCB_H

// Auto-generated from CraftingInventoryCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <vector>

class EntityPlayer;
class IInventory;

class CraftingInventoryCB {
public:
	CraftingInventoryCB();
	virtual ~CraftingInventoryCB();

	virtual void onCraftGuiClosed(EntityPlayer* var1);
	virtual void onCraftMatrixChanged(IInventory* var1);

public: // was protected
	std::vector<void*> /* PORT: determine element type */ m_unusedList;

};

#endif // CRAFTINGINVENTORYCB_H