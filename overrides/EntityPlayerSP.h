#ifndef ENTITYPLAYERSP_H
#define ENTITYPLAYERSP_H

// Auto-generated from EntityPlayerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityPlayer.h"

class Entity;
class IInventory;
class Minecraft;
class MovementInput;
class NBTTagCompound;
class Session;
class TileEntityFurnace;
class TileEntitySign;
class World;

class EntityPlayerSP : public EntityPlayer {
public:
	EntityPlayerSP(Minecraft* var1, World* var2, Session* var3, int var4);
	virtual ~EntityPlayerSP();

	MovementInput* movementInput;
	Minecraft* mc;
	int timeUntilPortal;
	bool inPortal;
	float timeInPortal;
	float prevTimeInPortal;

	virtual void updateEntityActionState();
	virtual void onLivingUpdate();
	virtual void resetMovementInput();
	virtual void setKeyState(int var1, bool var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void displayGUIChest(IInventory* var1);
	virtual void displayGUIEditSign(TileEntitySign* var1);
	virtual void displayWorkbenchGUI();
	virtual void displayGUIFurnace(TileEntityFurnace* var1);
	virtual void onItemPickup(Entity* var1, int var2);
	virtual int getPlayerArmorValue();
	virtual void useCurrentItemOnEntity(Entity* var1);
	virtual void sendChatMessage(const JavaString& var1);
	virtual void onUpdateClientPlayer();
	virtual bool isSneaking();
	virtual void setInPortal();
	virtual void setHealth(int var1);
	virtual void respawnPlayer();
};

#endif // ENTITYPLAYERSP_H