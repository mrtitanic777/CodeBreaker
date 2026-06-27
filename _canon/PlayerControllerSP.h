#ifndef PLAYERCONTROLLERSP_H
#define PLAYERCONTROLLERSP_H

// Auto-generated from PlayerControllerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "PlayerController.h"

class EntityPlayer;
class Minecraft;
class World;

class PlayerControllerSP : public PlayerController {
public:
	PlayerControllerSP(Minecraft* var1);
	virtual ~PlayerControllerSP();

	int currentBlockX;
	int currentBlockY;
	int currentBlockZ;
	float curBlockDamage;
	float prevBlockDamage;
	float stepSoundTickCounter;
	int blockHitDelay;

	virtual void flipPlayer(EntityPlayer* var1);
	virtual bool sendBlockRemoved(int var1, int var2, int var3, int var4);
	virtual void clickBlock(int var1, int var2, int var3, int var4);
	virtual void resetBlockRemoving();
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void updateBlockHitOverlay(float var1);
	virtual float getBlockReachDistance();
	virtual void addPendingConnection(World* var1);
	virtual void resetBlockRemovingSound();

	static void initStatics();
};

#endif // PLAYERCONTROLLERSP_H