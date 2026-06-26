#include "stdafx.h"
// Hand-ported from TileEntityChest.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Chest tile-entity: a 27-slot IInventory backed by an ItemStack** array
// (empty slots are NULL). Idioms follow the just-ported TileEntityFurnace and
// the converted TileEntityChest:
//   - slots are pointers, so use '->' not '.'
//   - onInventoryChanged() is declared in BOTH TileEntity and IInventory
//     (diamond) -> an unqualified call is ambiguous (C2385); the Alpha Java
//     chest does not call it at all, so we don't either.
//   - getInvName() returns a const std::string& -> back it with a stable static
//     (the Java returned a String literal).

#include "TileEntityChest.h"
#include "Common.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTBase.h"
#include "TileEntity.h"
#include <string>

TileEntityChest::TileEntityChest()
	: m_field_827_a(new ItemStack*[36]())
	, m_field_827_a_count(36)   // PORT: array-length companion (Java new ItemStack[36])
{
}

TileEntityChest::~TileEntityChest() {
	delete[] m_field_827_a;
	m_field_827_a = NULL;
}

int TileEntityChest::getSizeInventory() {
	return 27;
}

ItemStack* TileEntityChest::getStackInSlot(int var1) {
	return m_field_827_a[var1];
}

ItemStack* TileEntityChest::decrStackSize(int var1, int var2) {
	if(m_field_827_a[var1] != NULL) {
		ItemStack* var3 = NULL;
		if(m_field_827_a[var1]->stackSize <= var2) {
			var3 = m_field_827_a[var1];
			m_field_827_a[var1] = NULL;
			return var3;
		} else {
			var3 = m_field_827_a[var1]->splitStack(var2);
			if(m_field_827_a[var1]->stackSize == 0) {
				m_field_827_a[var1] = NULL;
			}

			return var3;
		}
	} else {
		return NULL;
	}
}

void TileEntityChest::setInventorySlotContents(int var1, ItemStack* var2) {
	m_field_827_a[var1] = var2;
	if(var2 != NULL && var2->stackSize > getInventoryStackLimit()) {
		var2->stackSize = getInventoryStackLimit();
	}

}

const std::string& TileEntityChest::getInvName() {
	// PORT: return a reference to a stable static (Java returned a literal).
	static const std::string s_name = "Chest";
	return s_name;
}

void TileEntityChest::readFromNBT(NBTTagCompound* var1) {
	NBTTagList* var5 = var1->getTagList("Items");
	delete[] m_field_827_a;
	m_field_827_a = new ItemStack*[27]();
	m_field_827_a_count = 27;

	for(int var2 = 0; var2 < var5->tagCount(); ++var2) {
		NBTTagCompound* var3 = (NBTTagCompound*)var5->tagAt(var2);
		int var4 = var3->getByte("Slot") & 255;
		if(var4 >= 0 && var4 < m_field_827_a_count) {
			m_field_827_a[var4] = new ItemStack(var3);
		}
	}

}

void TileEntityChest::writeToNBT(NBTTagCompound* var1) {
	var1->setString("id", "Chest");
	NBTTagList* var2 = new NBTTagList();

	for(int var3 = 0; var3 < m_field_827_a_count; ++var3) {
		if(m_field_827_a[var3] != NULL) {
			NBTTagCompound* var4 = new NBTTagCompound();
			var4->setByte("Slot", (unsigned char)var3);
			m_field_827_a[var3]->writeToNBT(var4);
			var2->setTag(var4);
		}
	}

	var1->setTag("Items", var2);
}

int TileEntityChest::getInventoryStackLimit() {
	return 64;
}

// PORT: IInventory::onInventoryChanged() is pure-virtual; TileEntity has a
// concrete one but (multiple inheritance) it does NOT satisfy IInventory's slot.
// Declare+define the override (header companion in handwritten_alpha) so
// TileEntityChest is concrete and `new TileEntityChest` (Debug.cpp) compiles.
void TileEntityChest::onInventoryChanged() {
	TileEntity::onInventoryChanged();
}
