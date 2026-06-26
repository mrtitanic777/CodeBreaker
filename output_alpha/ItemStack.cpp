#include "stdafx.h"
// Auto-generated from ItemStack.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemStack.h"
#include "Common.h"
#include "Block.h"
#include "Entity.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "Item.h"
#include "NBTTagCompound.h"
#include "World.h"

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
		readFromNBT(var1);
}

ItemStack::~ItemStack() {
	// No dynamic resources
}

ItemStack* ItemStack::splitStack(int var1) {
		stackSize -= var1;
		return new ItemStack(itemID, var1, itemDamage);
}

Item* ItemStack::getItem() {
		return Item::itemsList[itemID];
}

int ItemStack::getIconIndex() {
		return getItem()->getIconIndex(this);
}

bool ItemStack::useItem(EntityPlayer* var1, World* var2, int var3, int var4, int var5, int var6) {
		return getItem()->onItemUse(this, var1, var2, var3, var4, var5, var6);
}

float ItemStack::getStrVsBlock(Block* var1) {
		return getItem()->getStrVsBlock(this, var1);
}

ItemStack* ItemStack::useItemRightClick(World* var1, EntityPlayer* var2) {
		return getItem()->onItemRightClick(this, var1, var2);
}

NBTTagCompound* ItemStack::writeToNBT(NBTTagCompound* var1) {
		var1->setShort("id", (short)itemID);
		var1->setByte("Count", (unsigned char)stackSize);
		var1->setShort("Damage", (short)itemDamage);
		return var1;
}

void ItemStack::readFromNBT(NBTTagCompound* var1) {
		itemID = var1->getShort("id");
		stackSize = var1->getByte("Count");
		itemDamage = var1->getShort("Damage");
}

int ItemStack::getMaxStackSize() {
		return getItem()->getItemStackLimit();
}

int ItemStack::getMaxDamage() {
		return Item::itemsList[itemID]->getMaxDamage();
}

void ItemStack::damageItem(int var1) {
		itemDamage += var1;
		if(itemDamage > getMaxDamage()) {
			--stackSize;
			if(stackSize < 0) {
				stackSize = 0;
			}

			itemDamage = 0;
		}

}

void ItemStack::hitEntity(EntityLiving* var1) {
		Item::itemsList[itemID]->hitEntity(this, var1);
}

void ItemStack::hitBlock(int var1, int var2, int var3, int var4) {
		Item::itemsList[itemID]->hitBlock(this, var1, var2, var3, var4);
}

int ItemStack::getDamageVsEntity(Entity* var1) {
		return Item::itemsList[itemID]->getDamageVsEntity(var1);
}

bool ItemStack::canHarvestBlock(Block* var1) {
		return Item::itemsList[itemID]->canHarvestBlock(var1);
}

void ItemStack::func_1097_a(EntityPlayer* var1) {
	// TODO: implement
}

void ItemStack::useItemOnEntity(EntityLiving* var1) {
		Item::itemsList[itemID]->func_4019_b(this, var1);
}

ItemStack* ItemStack::copy() {
		return new ItemStack(itemID, stackSize, itemDamage);
}
