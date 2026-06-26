#ifndef ITEMSTACK_H
#define ITEMSTACK_H

// Auto-generated from ItemStack.java
// Target: Xbox 360 / C++03 / DirectX 9

class Block;
class Entity;
class EntityLiving;
class EntityPlayer;
class Item;
class NBTTagCompound;
class World;

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

	virtual ItemStack* splitStack(int var1);
	virtual Item* getItem();
	virtual int getIconIndex();
	virtual bool useItem(EntityPlayer* var1, World* var2, int var3, int var4, int var5, int var6);
	virtual float getStrVsBlock(Block* var1);
	virtual ItemStack* useItemRightClick(World* var1, EntityPlayer* var2);
	virtual NBTTagCompound* writeToNBT(NBTTagCompound* var1);
	virtual void readFromNBT(NBTTagCompound* var1);
	virtual int getMaxStackSize();
	virtual int getMaxDamage();
	virtual void damageItem(int var1);
	virtual void hitEntity(EntityLiving* var1);
	virtual void hitBlock(int var1, int var2, int var3, int var4);
	virtual int getDamageVsEntity(Entity* var1);
	virtual bool canHarvestBlock(Block* var1);
	virtual void func_1097_a(EntityPlayer* var1);
	virtual void useItemOnEntity(EntityLiving* var1);
	virtual ItemStack* copy();

};

#endif // ITEMSTACK_H