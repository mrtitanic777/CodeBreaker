#ifndef PLAYERCONTROLLERSP_H
#define PLAYERCONTROLLERSP_H

// Auto-generated from PlayerControllerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"

class Minecraft;
class MobSpawner;
class World;

class PlayerControllerSP : public PlayerController {
public:
	PlayerControllerSP();
	PlayerControllerSP(Minecraft* var1);
	virtual ~PlayerControllerSP();

	virtual bool sendBlockRemoved(int var1, int var2, int var3);
	virtual void clickBlock(int var1, int var2, int var3);
	virtual void resetBlockRemoving();
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void setPartialTime(float var1);
	virtual float getBlockReachDistance();
	virtual void onWorldChange(World* var1);
	virtual void onUpdate();

private:
	int m_curBlockX;
	int m_curBlockY;
	int m_curBlockZ;
	float m_curBlockDamage;
	float m_prevBlockDamage;
	float m_blockDestroySoundCounter;
	int m_blockHitWait;
	MobSpawner* m_mobSpawner;

};

#endif // PLAYERCONTROLLERSP_H