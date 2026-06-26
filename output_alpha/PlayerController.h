#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

// Auto-generated from PlayerController.java
// Target: Xbox 360 / C++03 / DirectX 9

class Entity;
class EntityPlayer;
class ItemStack;
class Minecraft;
class World;

class PlayerController {
public:
	PlayerController();
	PlayerController(Minecraft* var1);
	virtual ~PlayerController();

	bool m_field_1064_b;

	virtual void func_717_a(World* var1);
	virtual void clickBlock(int var1, int var2, int var3, int var4);
	virtual bool sendBlockRemoved(int var1, int var2, int var3, int var4);
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void func_6468_a();
	virtual void func_6467_a(float var1);
	virtual float getBlockReachDistance();
	virtual bool sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3);
	virtual void flipPlayer(EntityPlayer* var1);
	virtual void func_6474_c();
	virtual bool func_6469_d();
	virtual void func_6473_b(EntityPlayer* var1);
	virtual bool sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7);
	virtual EntityPlayer* func_4087_b(World* var1);
	virtual void func_6475_a(EntityPlayer* var1, Entity* var2);
	virtual void func_6472_b(EntityPlayer* var1, Entity* var2);

public: // was protected
	Minecraft* mc;

};

#endif // PLAYERCONTROLLER_H