#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

// Auto-generated from EntityPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"
#include <string>

class Block;
class Entity;
class EntityFish;
class EntityItem;
class IInventory;
class InventoryPlayer;
class ItemStack;
class NBTTagCompound;
class TileEntityFurnace;
class TileEntitySign;
class World;

class EntityPlayer : public EntityLiving {
public:
	EntityPlayer();
	EntityPlayer(World* var1);
	virtual ~EntityPlayer();

	InventoryPlayer* m_inventory;
	unsigned char m_field_9371_f;
	int m_score;
	float m_field_775_e;
	float m_field_774_f;
	bool m_field_9369_j;
	int m_field_9368_k;
	std::string m_field_771_i;
	int m_dimension;
	EntityFish* m_fishEntity;

	virtual void func_350_p();
	virtual void preparePlayerToSpawn();
	virtual void onLivingUpdate();
	virtual int func_6417_t();
	virtual void onDeath(Entity* var1);
	virtual void addToPlayerScore(Entity* var1, int var2);
	virtual void dropPlayerItem(ItemStack* var1);
	virtual void dropPlayerItemWithRandomChoice(ItemStack* var1, bool var2);
	virtual float getCurrentPlayerStrVsBlock(Block* var1);
	virtual bool canHarvestBlock(Block* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void displayGUIChest(IInventory* var1);
	virtual void displayWorkbenchGUI();
	virtual void func_443_a_(Entity* var1, int var2);
	virtual float func_373_s();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void displayGUIFurnace(TileEntityFurnace* var1);
	virtual void displayGUIEditSign(TileEntitySign* var1);
	virtual void func_6415_a_(Entity* var1);
	virtual ItemStack* getCurrentEquippedItem();
	virtual void destroyCurrentEquippedItem();
	virtual double func_388_v();
	virtual void func_457_w();
	virtual void attackTargetEntityWithCurrentItem(Entity* var1);
	virtual void func_9367_r();

public: // was protected
	virtual void func_418_b_();
	virtual void joinEntityItemWithWorld(EntityItem* var1);
	virtual void damageEntity(int var1);

public: // was private
	int m_field_781_a;

	void func_451_h(Entity* var1);

};

#endif // ENTITYPLAYER_H