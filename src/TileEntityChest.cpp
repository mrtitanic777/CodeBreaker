#include "stdafx.h"
// Auto-generated from TileEntityChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntityChest.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include <string>
#include "ItemStack.h"
#include "NBTTagList.h"

TileEntityChest::TileEntityChest()
	: m_chestContents(new ItemStack[36])
{
}

TileEntityChest::~TileEntityChest() {
	delete[] m_chestContents;
	m_chestContents = NULL;
}

int TileEntityChest::getSizeInventory() {
		return 27;
}

ItemStack TileEntityChest::getStackInSlot(int var1) {
		return m_chestContents[var1];
}

ItemStack TileEntityChest::decrStackSize(int var1, int var2) {
		if(m_chestContents[var1] != NULL) {
			ItemStack var3;
			if(m_chestContents[var1].stackSize <= var2) {
				var3 = m_chestContents[var1];
				m_chestContents[var1] = NULL;
				return var3;
			} else {
				var3 = m_chestContents[var1].splitStack(var2);
				if(m_chestContents[var1].stackSize == 0) {
					m_chestContents[var1] = NULL;
				}

				return var3;
			}
		} else {
			return NULL;
		}
}

void TileEntityChest::setInventorySlotContents(int var1, const ItemStack& var2) {
		m_chestContents[var1] = var2;
		if(var2 != NULL && var2.stackSize > 64) {
			var2.stackSize = 64;
		}

}

const std::string& TileEntityChest::getInvName() {
		return "Chest";
}

void TileEntityChest::readFromNBT(NBTTagCompound* var1) {
		NBTTagList* var5 = var1->getTagList("Items");
		m_chestContents = new ItemStack[27];

		for(int var2 = 0; var2 < var5->tagCount(); ++var2) {
			NBTTagCompound* var3 = (NBTTagCompound*)var5->tagAt(var2);
			int var4 = var3->getByte("Slot") & 255;
			if(var4 >= 0 && var4 < 27) {
				m_chestContents[var4] = ItemStack(var3);
			}
		}

}

void TileEntityChest::writeToNBT(NBTTagCompound* var1) {
		var1->setString("id", "Chest");
		NBTTagList* var2 = new NBTTagList();

		for(int var3 = 0; var3 < 27; ++var3) {
			if(m_chestContents[var3] != NULL) {
				NBTTagCompound* var4 = new NBTTagCompound();
				var4->setByte("Slot", (unsigned char)var3);
				m_chestContents[var3].writeToNBT(var4);
				var2->setTag(var4);
			}
		}

		var1->setTag("Items", var2);
}

int TileEntityChest::getInventoryStackLimit() {
		return 64;
}
