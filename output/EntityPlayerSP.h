#ifndef ENTITYPLAYERSP_H
#define ENTITYPLAYERSP_H

// Auto-generated from EntityPlayerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayer.h"
#include <string>

class Entity;
class IInventory;
class Minecraft;
class MovementInput;
class NBTTagCompound;
class Session;
class World;

class EntityPlayerSP : public EntityPlayer {
public:
	EntityPlayerSP();
	EntityPlayerSP(Minecraft* var1, World* var2, Session* var3);
	virtual ~EntityPlayerSP();

	MovementInput* m_movementInput;

	virtual void updatePlayerActionState();
	virtual void onLivingUpdate();
	virtual void displayGUIChest(IInventory* var1);
	virtual void displayWorkbenchGUI();
	virtual void displayGUIInventory();
	virtual void onItemPickup(Entity* var1);

public: // was protected
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual const std::string& getEntityString();

public: // was private
	Minecraft* mc;

};

#endif // ENTITYPLAYERSP_H