#ifndef PLAYERCONTROLLERTEST_H
#define PLAYERCONTROLLERTEST_H

// Auto-generated from PlayerControllerTest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"

class EntityPlayer;
class Minecraft;
class World;

class PlayerControllerTest : public PlayerController {
public:
	PlayerControllerTest();
	PlayerControllerTest(Minecraft* var1);
	virtual ~PlayerControllerTest();

	virtual void func_6473_b(EntityPlayer* var1);
	virtual bool func_6469_d();
	virtual void func_717_a(World* var1);
	virtual void func_6474_c();

};

#endif // PLAYERCONTROLLERTEST_H