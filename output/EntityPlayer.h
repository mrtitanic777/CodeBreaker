#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

// Auto-generated from EntityPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityLiving.h"
#include <string>

class Entity;
class IInventory;
class InventoryPlayer;
class ItemStack;
class NBTTagCompound;
class World;

class EntityPlayer : public EntityLiving {
public:
	EntityPlayer();
	EntityPlayer(World* var1);
	virtual ~EntityPlayer();

	InventoryPlayer* m_inventory;
	unsigned char m_unusedByte;
	int m_getScore;
	float m_prevCameraYaw;
	float m_cameraYaw;
	float cameraPitch;

	virtual void preparePlayerToSpawn();
	virtual void onLivingUpdate();
	virtual void onDeath(Entity* var1);
	virtual void setEntityDead();
	virtual void dropPlayerItem(ItemStack* var1);
	virtual void dropPlayerItemWithRandomChoice(ItemStack* var1, bool var2);
	virtual void displayGUIChest(IInventory* var1);
	virtual void displayWorkbenchGUI();
	virtual void onItemPickup(Entity* var1);
	virtual bool attackEntityFrom(Entity* var1, int var2);

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();
	virtual float getEyeHeight();

};

#endif // ENTITYPLAYER_H