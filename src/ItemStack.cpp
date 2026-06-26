#include "stdafx.h"
// Auto-generated from ItemStack.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemStack.h"
#include "Common.h"
#include "Block.h"
#include "Item.h"
#include "NBTTagCompound.h"

ItemStack::ItemStack()
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
}

ItemStack::ItemStack(Block* var1)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
  // PORT: constructor delegation  // was: this((Block*)var1, 1);
}

ItemStack::ItemStack(Block* var1, int var2)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
  // PORT: constructor delegation  // was: this(var1->blockID, var2);
}

ItemStack::ItemStack(Item* var1)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
  // PORT: constructor delegation  // was: this((Item*)var1, 1);
}

ItemStack::ItemStack(Item* var1, int var2)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
  // PORT: constructor delegation  // was: this(var1->shiftedIndex, var2);
}

ItemStack::ItemStack(int var1)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
  // PORT: constructor delegation  // was: this(var1, 1);
}

ItemStack::ItemStack(int var1, int var2)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
		stackSize = 0;
		itemID = var1;
		stackSize = var2;
}

ItemStack::ItemStack(int var1, int var2, int var3)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
		stackSize = 0;
		itemID = var1;
		stackSize = var2;
		itemDamage = var3;
}

ItemStack::ItemStack(NBTTagCompound* var1)
	: stackSize(0)
	, m_animationsToGo(0)
	, itemID(0)
	, itemDamage(0)
{
		stackSize = 0;
		itemID = var1->getShort("id");
		stackSize = var1->getByte("Count");
		itemDamage = var1->getShort("Damage");
}

ItemStack::~ItemStack() {
	// No dynamic resources
}

ItemStack ItemStack::splitStack(int var1) {
		stackSize -= var1;
		return ItemStack(itemID, var1, itemDamage);
}

Item* ItemStack::getItem() {
		return Item::itemsList[itemID];
}

NBTTagCompound* ItemStack::writeToNBT(NBTTagCompound* var1) {
		var1->setShort("id", (short)itemID);
		var1->setByte("Count", (unsigned char)stackSize);
		var1->setShort("Damage", (short)itemDamage);
		return var1;
}

int ItemStack::isItemStackDamageable() {
		return Item::itemsList[itemID].getMaxDamage();
}

void ItemStack::damageItem(int var1) {
		itemDamage += var1;
		if(itemDamage > isItemStackDamageable()) {
			--stackSize;
			if(stackSize < 0) {
				stackSize = 0;
			}

			itemDamage = 0;
		}

}
