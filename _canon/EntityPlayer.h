#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

// Auto-generated from EntityPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityLiving.h"

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
	EntityPlayer(World* var1);
	virtual ~EntityPlayer();

	InventoryPlayer* inventory;
	unsigned char blockBreakingPower;
	int score;
	float prevCameraYaw;
	float cameraYaw;
	bool isSwinging;
	int swingProgressInt;
	JavaString field_771_i;
	int dimension;
	int damageRemainder;
	EntityFish* fishEntity;

	virtual void updateRidden();
	virtual void preparePlayerToSpawn();
	virtual void updateEntityActionState();
	virtual void onLivingUpdate();
	virtual void collideWithPlayer(Entity* var1);
	virtual int getScore();
	virtual void onDeath(Entity* var1);
	virtual void addToPlayerScore(Entity* var1, int var2);
	virtual void dropPlayerItem(ItemStack* var1);
	virtual void dropPlayerItemWithRandomChoice(ItemStack* var1, bool var2);
	virtual void joinEntityItemWithWorld(EntityItem* var1);
	virtual float getCurrentPlayerStrVsBlock(Block* var1);
	virtual bool canHarvestBlock(Block* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void displayGUIChest(IInventory* var1);
	virtual void displayWorkbenchGUI();
	virtual void onItemPickup(Entity* var1, int var2);
	virtual float getEyeHeight();
	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void damageEntity(int var1);
	virtual void displayGUIFurnace(TileEntityFurnace* var1);
	virtual void displayGUIEditSign(TileEntitySign* var1);
	virtual void useCurrentItemOnEntity(Entity* var1);
	virtual ItemStack* getCurrentEquippedItem();
	virtual void destroyCurrentEquippedItem();
	virtual double getYOffset();
	virtual void swingItem();
	virtual void attackTargetEntityWithCurrentItem(Entity* var1);
	virtual void respawnPlayer();
};

#endif // ENTITYPLAYER_H