#include "stdafx.h"
// Hand-ported from TileEntityFurnace.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Pure furnace smelting LOGIC + IInventory. The slots are an ItemStack**
// array (3 entries: 0=input, 1=fuel, 2=output); empty slots are NULL.
// Idioms follow the converted TileEntityChest and handwritten InventoryPlayer.

#include "TileEntityFurnace.h"
#include "Common.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTBase.h"
#include "TileEntity.h"
#include "World.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"
#include "BlockFurnace.h"
#include <string>

TileEntityFurnace::TileEntityFurnace()
	: m_furnaceItemStacks(new ItemStack*[3]())
	, m_furnaceBurnTime(0)
	, m_currentItemBurnTime(0)
	, m_furnaceCookTime(0)
	, m_furnaceItemStacks_count(3)   // PORT: array-length companion or loop bounds are garbage
{
}

TileEntityFurnace::~TileEntityFurnace() {
	delete[] m_furnaceItemStacks;
	m_furnaceItemStacks = NULL;
}

int TileEntityFurnace::getSizeInventory() {
	return m_furnaceItemStacks_count;
}

ItemStack* TileEntityFurnace::getStackInSlot(int var1) {
	return m_furnaceItemStacks[var1];
}

ItemStack* TileEntityFurnace::decrStackSize(int var1, int var2) {
	if(m_furnaceItemStacks[var1] != NULL) {
		ItemStack* var3 = NULL;
		if(m_furnaceItemStacks[var1]->stackSize <= var2) {
			var3 = m_furnaceItemStacks[var1];
			m_furnaceItemStacks[var1] = NULL;
			return var3;
		} else {
			var3 = m_furnaceItemStacks[var1]->splitStack(var2);
			if(m_furnaceItemStacks[var1]->stackSize == 0) {
				m_furnaceItemStacks[var1] = NULL;
			}

			return var3;
		}
	} else {
		return NULL;
	}
}

void TileEntityFurnace::setInventorySlotContents(int var1, ItemStack* var2) {
	m_furnaceItemStacks[var1] = var2;
	if(var2 != NULL && var2->stackSize > getInventoryStackLimit()) {
		var2->stackSize = getInventoryStackLimit();
	}

}

const std::string& TileEntityFurnace::getInvName() {
	// PORT: return a reference to a stable static (Java returned a literal).
	static const std::string s_name = "Chest";
	return s_name;
}

void TileEntityFurnace::readFromNBT(NBTTagCompound* var1) {
	TileEntity::readFromNBT(var1);
	NBTTagList* var2 = var1->getTagList("Items");
	delete[] m_furnaceItemStacks;
	m_furnaceItemStacks = new ItemStack*[getSizeInventory()]();
	m_furnaceItemStacks_count = getSizeInventory();

	for(int var3 = 0; var3 < var2->tagCount(); ++var3) {
		NBTTagCompound* var4 = (NBTTagCompound*)var2->tagAt(var3);
		int var5 = var4->getByte("Slot") & 255;
		if(var5 >= 0 && var5 < getSizeInventory()) {
			m_furnaceItemStacks[var5] = new ItemStack(var4);
		}
	}

	m_furnaceBurnTime = var1->getShort("BurnTime");
	m_furnaceCookTime = var1->getShort("CookTime");
	m_currentItemBurnTime = getItemBurnTime(m_furnaceItemStacks[1]);
}

void TileEntityFurnace::writeToNBT(NBTTagCompound* var1) {
	// PORT: base TileEntity::writeToNBT now needs an id; write it explicitly
	// (no reflection on Xbox 360) then persist coords + furnace state.
	var1->setString("id", "Furnace");
	var1->setInteger("x", xCoord);
	var1->setInteger("y", yCoord);
	var1->setInteger("z", zCoord);
	var1->setShort("BurnTime", (short)m_furnaceBurnTime);
	var1->setShort("CookTime", (short)m_furnaceCookTime);
	NBTTagList* var2 = new NBTTagList();

	for(int var3 = 0; var3 < getSizeInventory(); ++var3) {
		if(m_furnaceItemStacks[var3] != NULL) {
			NBTTagCompound* var4 = new NBTTagCompound();
			var4->setByte("Slot", (unsigned char)var3);
			m_furnaceItemStacks[var3]->writeToNBT(var4);
			var2->setTag(var4);
		}
	}

	var1->setTag("Items", var2);
}

int TileEntityFurnace::getInventoryStackLimit() {
	return 64;
}

int TileEntityFurnace::getCookProgressScaled(int var1) {
	return m_furnaceCookTime * var1 / 200;
}

int TileEntityFurnace::getBurnTimeRemainingScaled(int var1) {
	if(m_currentItemBurnTime == 0) {
		m_currentItemBurnTime = 200;
	}

	return m_furnaceBurnTime * var1 / m_currentItemBurnTime;
}

bool TileEntityFurnace::isBurning() {
	return m_furnaceBurnTime > 0;
}

void TileEntityFurnace::updateEntity() {
	bool var1 = m_furnaceBurnTime > 0;
	bool var2 = false;
	if(m_furnaceBurnTime > 0) {
		--m_furnaceBurnTime;
	}

	if(!worldObj->m_multiplayerWorld) {
		if(m_furnaceBurnTime == 0 && canSmelt()) {
			m_currentItemBurnTime = m_furnaceBurnTime = getItemBurnTime(m_furnaceItemStacks[1]);
			if(m_furnaceBurnTime > 0) {
				var2 = true;
				if(m_furnaceItemStacks[1] != NULL) {
					--m_furnaceItemStacks[1]->stackSize;
					if(m_furnaceItemStacks[1]->stackSize == 0) {
						m_furnaceItemStacks[1] = NULL;
					}
				}
			}
		}

		if(isBurning() && canSmelt()) {
			++m_furnaceCookTime;
			if(m_furnaceCookTime == 200) {
				m_furnaceCookTime = 0;
				smeltItem();
				var2 = true;
			}
		} else {
			m_furnaceCookTime = 0;
		}

		if(var1 != (m_furnaceBurnTime > 0)) {
			var2 = true;
			BlockFurnace::updateFurnaceBlockState(m_furnaceBurnTime > 0, worldObj, xCoord, yCoord, zCoord);
		}
	}

	if(var2) {
		// PORT: onInventoryChanged() is declared in BOTH TileEntity and IInventory
		// (diamond) -> unqualified call is ambiguous (C2385). Use TileEntity's impl.
		TileEntity::onInventoryChanged();
	}

}

bool TileEntityFurnace::canSmelt() {
	if(m_furnaceItemStacks[0] == NULL) {
		return false;
	} else {
		int var1 = getSmeltingResultItem(m_furnaceItemStacks[0]->getItem()->shiftedIndex);
		return var1 < 0 ? false : (m_furnaceItemStacks[2] == NULL ? true : (m_furnaceItemStacks[2]->itemID != var1 ? false : (m_furnaceItemStacks[2]->stackSize < getInventoryStackLimit() && m_furnaceItemStacks[2]->stackSize < m_furnaceItemStacks[2]->getMaxStackSize() ? true : m_furnaceItemStacks[2]->stackSize < Item::itemsList[var1]->getItemStackLimit())));
	}
}

void TileEntityFurnace::smeltItem() {
	if(canSmelt()) {
		int var1 = getSmeltingResultItem(m_furnaceItemStacks[0]->getItem()->shiftedIndex);
		if(m_furnaceItemStacks[2] == NULL) {
			m_furnaceItemStacks[2] = new ItemStack(var1, 1);
		} else if(m_furnaceItemStacks[2]->itemID == var1) {
			++m_furnaceItemStacks[2]->stackSize;
		}

		--m_furnaceItemStacks[0]->stackSize;
		if(m_furnaceItemStacks[0]->stackSize <= 0) {
			m_furnaceItemStacks[0] = NULL;
		}

	}
}

int TileEntityFurnace::getSmeltingResultItem(int var1) {
	return var1 == Block::s_oreIron->blockID ? Item::s_ingotIron->shiftedIndex : (var1 == Block::s_oreGold->blockID ? Item::s_ingotGold->shiftedIndex : (var1 == Block::s_oreDiamond->blockID ? Item::s_diamond->shiftedIndex : (var1 == Block::s_sand->blockID ? Block::s_glass->blockID : (var1 == Item::s_porkRaw->shiftedIndex ? Item::s_porkCooked->shiftedIndex : (var1 == Item::s_fishRaw->shiftedIndex ? Item::s_fishCooked->shiftedIndex : (var1 == Block::s_cobblestone->blockID ? Block::s_stone->blockID : (var1 == Item::s_clay->shiftedIndex ? Item::s_brick->shiftedIndex : -1)))))));
}

int TileEntityFurnace::getItemBurnTime(ItemStack* var1) {
	if(var1 == NULL) {
		return 0;
	} else {
		int var2 = var1->getItem()->shiftedIndex;
		return var2 < 256 && Block::blocksList[var2]->m_blockMaterial == Material::s_wood ? 300 : (var2 == Item::s_stick->shiftedIndex ? 100 : (var2 == Item::s_coal->shiftedIndex ? 1600 : (var2 == Item::s_bucketLava->shiftedIndex ? 20000 : 0)));
	}
}

// PORT: declared in TileEntityFurnace.h to make the class concrete (IInventory's
// onInventoryChanged is pure-virtual; TileEntity has a concrete one but across the
// two bases it doesn't satisfy IInventory's slot). Forward to TileEntity's impl.
void TileEntityFurnace::onInventoryChanged() {
	TileEntity::onInventoryChanged();
}
