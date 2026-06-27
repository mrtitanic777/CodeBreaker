#ifndef PLAYERCONTROLLERTEST_H
#define PLAYERCONTROLLERTEST_H

// Auto-generated from PlayerControllerTest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "PlayerController.h"

class EntityPlayer;
class Minecraft;
class World;

class PlayerControllerTest : public PlayerController {
public:
	PlayerControllerTest(Minecraft* var1);
	virtual ~PlayerControllerTest();

	virtual void onPlayerCreated(EntityPlayer* var1);
	virtual bool shouldDrawHealthBar();
	virtual void addPendingConnection(World* var1);
	virtual void resetBlockRemovingSound();

	static void initStatics();
};

#endif // PLAYERCONTROLLERTEST_H