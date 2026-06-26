#ifndef ENTITYCLIENTPLAYERMP_H
#define ENTITYCLIENTPLAYERMP_H

// Auto-generated from EntityClientPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayerSP.h"
#include <string>

class Entity;
class EntityItem;
class InventoryPlayer;
class Minecraft;
class NetClientHandler;
class Session;
class World;

class EntityClientPlayerMP : public EntityPlayerSP {
public:
	EntityClientPlayerMP();
	EntityClientPlayerMP(Minecraft* var1, World* var2, Session* var3, NetClientHandler* var4);
	virtual ~EntityClientPlayerMP();

	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void heal(int var1);
	virtual void onUpdate();
	virtual void func_6420_o();
	virtual void func_4056_N();
	virtual void sendChatMessage(const std::string& var1);
	virtual void func_457_w();
	virtual void func_9367_r();

public: // was protected
	virtual void joinEntityItemWithWorld(EntityItem* var1);
	virtual void damageEntity(int var1);

public: // was private
	NetClientHandler* m_field_797_bg;
	int m_field_9380_bx;
	double m_field_9379_by;
	double m_field_9378_bz;
	double m_field_9377_bA;
	double m_field_9376_bB;
	float m_field_9385_bC;
	float m_field_9384_bD;
	InventoryPlayer* m_field_9383_bE;
	bool m_field_9382_bF;
	bool m_field_9381_bG;
	int m_field_12242_bI;

	void sendInventoryChanged();

};

#endif // ENTITYCLIENTPLAYERMP_H