#ifndef ITEMSTACK_H
#define ITEMSTACK_H

// Auto-generated from ItemStack.java
// Target: Xbox 360 / C++03 / DirectX 9

class Block;
class Item;
class NBTTagCompound;

class ItemStack {
public:
	ItemStack();
	ItemStack(Block* var1);
	ItemStack(Block* var1, int var2);
	ItemStack(Item* var1);
	ItemStack(Item* var1, int var2);
	ItemStack(int var1);
	ItemStack(int var1, int var2);
	ItemStack(int var1, int var2, int var3);
	ItemStack(NBTTagCompound* var1);
	virtual ~ItemStack();

	int stackSize;
	int m_animationsToGo;
	int itemID;
	int itemDamage;

	virtual ItemStack splitStack(int var1);
	virtual Item* getItem();
	virtual NBTTagCompound* writeToNBT(NBTTagCompound* var1);
	virtual int isItemStackDamageable();
	virtual void damageItem(int var1);

};

#endif // ITEMSTACK_H