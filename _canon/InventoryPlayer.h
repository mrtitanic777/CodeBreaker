#ifndef INVENTORYPLAYER_H
#define INVENTORYPLAYER_H

// Auto-generated from InventoryPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IInventory.h"

class Block;
class Entity;
class EntityPlayer;
class ItemStack;
class NBTTagList;

class InventoryPlayer : public IInventory {
public:
	InventoryPlayer(EntityPlayer* var1);
	virtual ~InventoryPlayer();

	JArray<ItemStack* > mainInventory;
	JArray<ItemStack* > armorInventory;
	JArray<ItemStack* > craftingInventory;
	int currentItem;
	EntityPlayer* player;
	ItemStack* draggingItemStack;
	bool inventoryChanged;

	virtual ItemStack* getCurrentItem();
	virtual int getInventorySlotContainItem(int var1);
	virtual int getFirstPartialMatchingStack(int var1);
	virtual int getFirstEmptyStack();
	virtual void setCurrentItem(int var1, bool var2);
	virtual void changeCurrentItem(int var1);
	virtual int addItemsToInventory(int var1, int var2);
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
	virtual JavaString getInvName();
	virtual int getInventoryStackLimit();
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool canHarvestBlock(Block* var1);
	virtual ItemStack* armorItemInSlot(int var1);
	virtual int getTotalArmorValue();
	virtual void damageArmor(int var1);
	virtual void dropAllItems();
	virtual void onInventoryChanged();
	virtual bool compareInventory(InventoryPlayer* var1);
	virtual bool compareItemStack(ItemStack* var1, ItemStack* var2);
	virtual InventoryPlayer* copyInventory();

	static void initStatics();
};

#endif // INVENTORYPLAYER_H