#ifndef PLAYERCONTROLLERMP_H
#define PLAYERCONTROLLERMP_H

// Auto-generated from PlayerControllerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"

class Entity;
class EntityPlayer;
class ItemStack;
class Minecraft;
class NetClientHandler;
class World;

class PlayerControllerMP : public PlayerController {
public:
	PlayerControllerMP();
	PlayerControllerMP(Minecraft* var1, NetClientHandler* var2);
	virtual ~PlayerControllerMP();

	virtual void flipPlayer(EntityPlayer* var1);
	virtual bool sendBlockRemoved(int var1, int var2, int var3, int var4);
	virtual void clickBlock(int var1, int var2, int var3, int var4);
	virtual void func_6468_a();
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void func_6467_a(float var1);
	virtual float getBlockReachDistance();
	virtual void func_717_a(World* var1);
	virtual void func_6474_c();
	virtual bool sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7);
	virtual bool sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3);
	virtual EntityPlayer* func_4087_b(World* var1);
	virtual void func_6472_b(EntityPlayer* var1, Entity* var2);
	virtual void func_6475_a(EntityPlayer* var1, Entity* var2);

public: // was private
	int m_field_9445_c;
	int m_field_9444_d;
	int m_field_9443_e;
	float m_field_9442_f;
	float m_field_1080_g;
	float m_field_9441_h;
	int m_field_9440_i;
	bool m_field_9439_j;
	NetClientHandler* m_field_9438_k;
	int m_field_1075_l;

	void func_730_e();

};

#endif // PLAYERCONTROLLERMP_H