#ifndef INVENTORYPLAYER_H
#define INVENTORYPLAYER_H

// Auto-generated from InventoryPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include <string>

class Block;
class Entity;
class EntityPlayer;
class ItemStack;
class NBTTagList;

class InventoryPlayer : public IInventory {
public:
	InventoryPlayer();
	InventoryPlayer(EntityPlayer* var1);
	virtual ~InventoryPlayer();

	ItemStack** m_mainInventory;
	ItemStack** m_armorInventory;
	ItemStack** m_craftingInventory;
	int m_currentItem;
	ItemStack* m_draggingItemStack;
	bool m_field_845_f;

	virtual ItemStack* getCurrentItem();
	virtual void setCurrentItem(int var1, bool var2);
	virtual void changeCurrentItem(int var1);
	virtual void decrementAnimations();
	virtual bool consumeInventoryItem(int var1);
	virtual bool addItemStackToInventory(ItemStack* var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual float getStrVsBlock(Block* var1);
	virtual NBTTagList* writeToNBT(NBTTagList* var1);
	virtual void readFromNBT(NBTTagList* var1);
	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual const std::string& getInvName();
	virtual int getInventoryStackLimit();
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool canHarvestBlock(Block* var1);
	virtual ItemStack* armorItemInSlot(int var1);
	virtual int getTotalArmorValue();
	virtual void damageArmor(int var1);
	virtual void dropAllItems();
	virtual void onInventoryChanged();
	virtual bool compareInventory(InventoryPlayer* var1);
	virtual InventoryPlayer* copyInventory();

public: // was private
	EntityPlayer* m_player;

	int getInventorySlotContainItem(int var1);
	int getFirstPartialMatchingStack(int var1);
	int getFirstEmptyStack();
	int addItemsToInventory(int var1, int var2);
	bool compareItemStack(ItemStack* var1, ItemStack* var2);

public:  // array-length companions
	int m_mainInventory_count;
	int m_armorInventory_count;
	int m_craftingInventory_count;

};

#endif // INVENTORYPLAYER_H