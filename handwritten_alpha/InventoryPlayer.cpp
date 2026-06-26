#include "stdafx.h"
// Hand-ported from InventoryPlayer.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Pure inventory LOGIC (no GL). The inventory is an ItemStack** array;
// empty slots are NULL pointers. Adapted from the converted Alpha output,
// using indev's working InventoryPlayer as the porting guide.

#include "InventoryPlayer.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "ItemStack.h"
#include "Item.h"
#include "ItemArmor.h"
#include "Block.h"
#include "Material.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NBTBase.h"
#include <string>

InventoryPlayer::InventoryPlayer()
	: m_mainInventory(new ItemStack*[37]())
	, m_armorInventory(new ItemStack*[4]())
	, m_craftingInventory(new ItemStack*[4]())
	, m_currentItem(0)
	, m_draggingItemStack(NULL)
	, m_field_845_f(false)
	, m_player(NULL)
	, m_mainInventory_count(36)   // PORT: array-length companions must be set or loop bounds are garbage
	, m_armorInventory_count(4)
	, m_craftingInventory_count(4)
{
}

InventoryPlayer::InventoryPlayer(EntityPlayer* var1)
	: m_mainInventory(new ItemStack*[37]())
	, m_armorInventory(new ItemStack*[4]())
	, m_craftingInventory(new ItemStack*[4]())
	, m_currentItem(0)
	, m_draggingItemStack(NULL)
	, m_field_845_f(false)
	, m_player(NULL)
	, m_mainInventory_count(36)
	, m_armorInventory_count(4)
	, m_craftingInventory_count(4)
{
	m_player = var1;
}

InventoryPlayer::~InventoryPlayer() {
	delete[] m_mainInventory;
	m_mainInventory = NULL;
	delete[] m_armorInventory;
	m_armorInventory = NULL;
	delete[] m_craftingInventory;
	m_craftingInventory = NULL;
	m_player = NULL;
	m_draggingItemStack = NULL;
}

ItemStack* InventoryPlayer::getCurrentItem() {
	return m_mainInventory[m_currentItem];
}

int InventoryPlayer::getInventorySlotContainItem(int var1) {
	for(int var2 = 0; var2 < 36; ++var2) {
		if(m_mainInventory[var2] != NULL && m_mainInventory[var2]->itemID == var1) {
			return var2;
		}
	}

	return -1;
}

int InventoryPlayer::getFirstPartialMatchingStack(int var1) {
	for(int var2 = 0; var2 < 36; ++var2) {
		if(m_mainInventory[var2] != NULL && m_mainInventory[var2]->itemID == var1 && m_mainInventory[var2]->stackSize < m_mainInventory[var2]->getMaxStackSize() && m_mainInventory[var2]->stackSize < getInventoryStackLimit()) {
			return var2;
		}
	}

	return -1;
}

int InventoryPlayer::getFirstEmptyStack() {
	for(int var1 = 0; var1 < 36; ++var1) {
		if(m_mainInventory[var1] == NULL) {
			return var1;
		}
	}

	return -1;
}

void InventoryPlayer::setCurrentItem(int var1, bool var2) {
	int var3 = getInventorySlotContainItem(var1);
	if(var3 >= 0 && var3 < 9) {
		m_currentItem = var3;
	}
}

void InventoryPlayer::changeCurrentItem(int var1) {
	if(var1 > 0) {
		var1 = 1;
	}

	if(var1 < 0) {
		var1 = -1;
	}

	for(m_currentItem -= var1; m_currentItem < 0; m_currentItem += 9) {
	}

	while(m_currentItem >= 9) {
		m_currentItem -= 9;
	}

}

int InventoryPlayer::addItemsToInventory(int var1, int var2) {
	int var3 = getFirstPartialMatchingStack(var1);
	if(var3 < 0) {
		var3 = getFirstEmptyStack();
	}

	if(var3 < 0) {
		return var2;
	} else {
		if(m_mainInventory[var3] == NULL) {
			m_mainInventory[var3] = new ItemStack(var1, 0);
		}

		int var4 = var2;
		if(var2 > m_mainInventory[var3]->getMaxStackSize() - m_mainInventory[var3]->stackSize) {
			var4 = m_mainInventory[var3]->getMaxStackSize() - m_mainInventory[var3]->stackSize;
		}

		if(var4 > getInventoryStackLimit() - m_mainInventory[var3]->stackSize) {
			var4 = getInventoryStackLimit() - m_mainInventory[var3]->stackSize;
		}

		if(var4 == 0) {
			return var2;
		} else {
			var2 -= var4;
			m_mainInventory[var3]->stackSize += var4;
			m_mainInventory[var3]->m_animationsToGo = 5;
			return var2;
		}
	}
}

void InventoryPlayer::decrementAnimations() {
	for(int var1 = 0; var1 < 36; ++var1) {
		if(m_mainInventory[var1] != NULL && m_mainInventory[var1]->m_animationsToGo > 0) {
			--m_mainInventory[var1]->m_animationsToGo;
		}
	}

}

bool InventoryPlayer::consumeInventoryItem(int var1) {
	int var2 = getInventorySlotContainItem(var1);
	if(var2 < 0) {
		return false;
	} else {
		if(--m_mainInventory[var2]->stackSize <= 0) {
			m_mainInventory[var2] = NULL;
		}

		return true;
	}
}

bool InventoryPlayer::addItemStackToInventory(ItemStack* var1) {
	if(var1->itemDamage == 0) {
		var1->stackSize = addItemsToInventory(var1->itemID, var1->stackSize);
		if(var1->stackSize == 0) {
			return true;
		}
	}

	int var2 = getFirstEmptyStack();
	if(var2 >= 0) {
		m_mainInventory[var2] = var1;
		m_mainInventory[var2]->m_animationsToGo = 5;
		return true;
	} else {
		return false;
	}
}

ItemStack* InventoryPlayer::decrStackSize(int var1, int var2) {
	// The inventory is an array of ItemStack pointers; select main vs armor.
	ItemStack** var3 = m_mainInventory;
	if(var1 >= 36) {
		var3 = m_armorInventory;
		var1 -= 36;
	}

	if(var3[var1] != NULL) {
		ItemStack* var4 = NULL;
		if(var3[var1]->stackSize <= var2) {
			var4 = var3[var1];
			var3[var1] = NULL;
			return var4;
		} else {
			var4 = var3[var1]->splitStack(var2);
			if(var3[var1]->stackSize == 0) {
				var3[var1] = NULL;
			}

			return var4;
		}
	} else {
		return NULL;
	}
}

void InventoryPlayer::setInventorySlotContents(int var1, ItemStack* var2) {
	ItemStack** var3 = m_mainInventory;
	if(var1 >= 36) {
		var1 -= 36;
		var3 = m_armorInventory;
	}

	if(var1 >= 4) {
		var1 -= 4;
		var3 = m_craftingInventory;
	}

	var3[var1] = var2;
}

float InventoryPlayer::getStrVsBlock(Block* var1) {
	float var2 = 1.0F;
	if(m_mainInventory[m_currentItem] != NULL) {
		var2 *= m_mainInventory[m_currentItem]->getStrVsBlock(var1);
	}

	return var2;
}

NBTTagList* InventoryPlayer::writeToNBT(NBTTagList* var1) {
	int var2;
	NBTTagCompound* var3 = NULL;
	for(var2 = 0; var2 < 36; ++var2) {
		if(m_mainInventory[var2] != NULL) {
			var3 = new NBTTagCompound();
			var3->setByte("Slot", (unsigned char)var2);
			m_mainInventory[var2]->writeToNBT(var3);
			var1->setTag(var3);
		}
	}

	for(var2 = 0; var2 < 4; ++var2) {
		if(m_armorInventory[var2] != NULL) {
			var3 = new NBTTagCompound();
			var3->setByte("Slot", (unsigned char)(var2 + 100));
			m_armorInventory[var2]->writeToNBT(var3);
			var1->setTag(var3);
		}
	}

	for(var2 = 0; var2 < 4; ++var2) {
		if(m_craftingInventory[var2] != NULL) {
			var3 = new NBTTagCompound();
			var3->setByte("Slot", (unsigned char)(var2 + 80));
			m_craftingInventory[var2]->writeToNBT(var3);
			var1->setTag(var3);
		}
	}

	return var1;
}

void InventoryPlayer::readFromNBT(NBTTagList* var1) {
	delete[] m_mainInventory;
	delete[] m_armorInventory;
	delete[] m_craftingInventory;
	m_mainInventory = new ItemStack*[36]();
	m_mainInventory_count = 36;
	m_armorInventory = new ItemStack*[4]();
	m_armorInventory_count = 4;
	m_craftingInventory = new ItemStack*[4]();
	m_craftingInventory_count = 4;

	for(int var2 = 0; var2 < var1->tagCount(); ++var2) {
		NBTTagCompound* var3 = (NBTTagCompound*)var1->tagAt(var2);
		int var4 = var3->getByte("Slot") & 255;
		if(var4 >= 0 && var4 < 36) {
			m_mainInventory[var4] = new ItemStack(var3);
		}

		if(var4 >= 80 && var4 < 4 + 80) {
			m_craftingInventory[var4 - 80] = new ItemStack(var3);
		}

		if(var4 >= 100 && var4 < 4 + 100) {
			m_armorInventory[var4 - 100] = new ItemStack(var3);
		}
	}

}

int InventoryPlayer::getSizeInventory() {
	return m_mainInventory_count + 4;
}

ItemStack* InventoryPlayer::getStackInSlot(int var1) {
	ItemStack** var2 = m_mainInventory;
	if(var1 >= 36) {
		var1 -= 36;
		var2 = m_armorInventory;
	}

	if(var1 >= 4) {
		var1 -= 4;
		var2 = m_craftingInventory;
	}

	return var2[var1];
}

const std::string& InventoryPlayer::getInvName() {
	static const std::string s_name = "Inventory";
	return s_name;
}

int InventoryPlayer::getInventoryStackLimit() {
	return 64;
}

int InventoryPlayer::getDamageVsEntity(Entity* var1) {
	ItemStack* var2 = getStackInSlot(m_currentItem);
	return var2 != NULL ? var2->getDamageVsEntity(var1) : 1;
}

bool InventoryPlayer::canHarvestBlock(Block* var1) {
	if(var1->m_blockMaterial != Material::s_rock && var1->m_blockMaterial != Material::s_iron && var1->m_blockMaterial != Material::s_builtSnow && var1->m_blockMaterial != Material::s_snow) {
		return true;
	} else {
		ItemStack* var2 = getStackInSlot(m_currentItem);
		return var2 != NULL ? var2->canHarvestBlock(var1) : false;
	}
}

ItemStack* InventoryPlayer::armorItemInSlot(int var1) {
	return m_armorInventory[var1];
}

int InventoryPlayer::getTotalArmorValue() {
	int var1 = 0;
	int var2 = 0;
	int var3 = 0;

	for(int var4 = 0; var4 < 4; ++var4) {
		// Java: m_armorInventory[var4].getItem() instanceof ItemArmor  ->  dynamic_cast
		if(m_armorInventory[var4] != NULL && dynamic_cast<ItemArmor*>(m_armorInventory[var4]->getItem()) != NULL) {
			int var5 = m_armorInventory[var4]->getMaxDamage();
			int var6 = m_armorInventory[var4]->itemDamage;
			int var7 = var5 - var6;
			var2 += var7;
			var3 += var5;
			int var8 = ((ItemArmor*)m_armorInventory[var4]->getItem())->m_damageReduceAmmount;
			var1 += var8;
		}
	}

	if(var3 == 0) {
		return 0;
	} else {
		return (var1 - 1) * var2 / var3 + 1;
	}
}

void InventoryPlayer::damageArmor(int var1) {
	for(int var2 = 0; var2 < 4; ++var2) {
		if(m_armorInventory[var2] != NULL && dynamic_cast<ItemArmor*>(m_armorInventory[var2]->getItem()) != NULL) {
			m_armorInventory[var2]->damageItem(var1);
			if(m_armorInventory[var2]->stackSize == 0) {
				m_armorInventory[var2]->func_1097_a(m_player);
				m_armorInventory[var2] = NULL;
			}
		}
	}

}

void InventoryPlayer::dropAllItems() {
	int var1;
	for(var1 = 0; var1 < 36; ++var1) {
		if(m_mainInventory[var1] != NULL) {
			m_player->dropPlayerItemWithRandomChoice(m_mainInventory[var1], true);
			m_mainInventory[var1] = NULL;
		}
	}

	for(var1 = 0; var1 < 4; ++var1) {
		if(m_armorInventory[var1] != NULL) {
			m_player->dropPlayerItemWithRandomChoice(m_armorInventory[var1], true);
			m_armorInventory[var1] = NULL;
		}
	}

}

void InventoryPlayer::onInventoryChanged() {
	m_field_845_f = true;
}

bool InventoryPlayer::compareInventory(InventoryPlayer* var1) {
	int var2;
	for(var2 = 0; var2 < 36; ++var2) {
		if(!compareItemStack(var1->m_mainInventory[var2], m_mainInventory[var2])) {
			return false;
		}
	}

	for(var2 = 0; var2 < 4; ++var2) {
		if(!compareItemStack(var1->m_armorInventory[var2], m_armorInventory[var2])) {
			return false;
		}
	}

	for(var2 = 0; var2 < 4; ++var2) {
		if(!compareItemStack(var1->m_craftingInventory[var2], m_craftingInventory[var2])) {
			return false;
		}
	}

	return true;
}

bool InventoryPlayer::compareItemStack(ItemStack* var1, ItemStack* var2) {
	return var1 == NULL && var2 == NULL ? true : (var1 != NULL && var2 != NULL ? var1->itemID == var2->itemID && var1->stackSize == var2->stackSize && var1->itemDamage == var2->itemDamage : false);
}

InventoryPlayer* InventoryPlayer::copyInventory() {
	InventoryPlayer* var1 = new InventoryPlayer(NULL);

	int var2;
	for(var2 = 0; var2 < 36; ++var2) {
		var1->m_mainInventory[var2] = m_mainInventory[var2] != NULL ? m_mainInventory[var2]->copy() : NULL;
	}

	for(var2 = 0; var2 < 4; ++var2) {
		var1->m_armorInventory[var2] = m_armorInventory[var2] != NULL ? m_armorInventory[var2]->copy() : NULL;
	}

	for(var2 = 0; var2 < 4; ++var2) {
		var1->m_craftingInventory[var2] = m_craftingInventory[var2] != NULL ? m_craftingInventory[var2]->copy() : NULL;
	}

	return var1;
}
