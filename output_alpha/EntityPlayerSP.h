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
class TileEntityFurnace;
class TileEntitySign;
class World;

class EntityPlayerSP : public EntityPlayer {
public:
	EntityPlayerSP();
	EntityPlayerSP(Minecraft* var1, World* var2, Session* var3, int var4);
	virtual ~EntityPlayerSP();

	MovementInput* m_field_787_a;
	int m_field_9373_b;
	float m_field_4134_c;
	float m_field_4133_d;

	virtual void func_418_b_();
	virtual void onLivingUpdate();
	virtual void func_458_k();
	virtual void func_460_a(int var1, bool var2);
	virtual void writeEntityToNBT(NBTTagCompound* var1);
	virtual void readEntityFromNBT(NBTTagCompound* var1);
	virtual void displayGUIChest(IInventory* var1);
	virtual void displayGUIEditSign(TileEntitySign* var1);
	virtual void displayWorkbenchGUI();
	virtual void displayGUIFurnace(TileEntityFurnace* var1);
	virtual void func_443_a_(Entity* var1, int var2);
	virtual int getPlayerArmorValue();
	virtual void func_6415_a_(Entity* var1);
	virtual void sendChatMessage(const std::string& var1);
	virtual void func_6420_o();
	virtual bool func_381_o();
	virtual void func_4039_q();
	virtual void setHealth(int var1);
	virtual void func_9367_r();

public: // was private
	Minecraft* mc;
	bool m_field_9374_bx;

};

#endif // ENTITYPLAYERSP_H