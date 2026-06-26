#ifndef PLAYERCONTROLLERSP_H
#define PLAYERCONTROLLERSP_H

// Auto-generated from PlayerControllerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"

class EntityPlayer;
class Minecraft;
class World;

class PlayerControllerSP : public PlayerController {
public:
	PlayerControllerSP();
	PlayerControllerSP(Minecraft* var1);
	virtual ~PlayerControllerSP();

	virtual void flipPlayer(EntityPlayer* var1);
	virtual bool sendBlockRemoved(int var1, int var2, int var3, int var4);
	virtual void clickBlock(int var1, int var2, int var3, int var4);
	virtual void func_6468_a();
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void func_6467_a(float var1);
	virtual float getBlockReachDistance();
	virtual void func_717_a(World* var1);
	virtual void func_6474_c();

public: // was private
	int m_field_1074_c;
	int m_field_1073_d;
	int m_field_1072_e;
	float m_field_1071_f;
	float m_field_1070_g;
	float m_field_1069_h;
	int m_field_1068_i;

};

#endif // PLAYERCONTROLLERSP_H