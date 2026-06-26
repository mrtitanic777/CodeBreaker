#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

// Auto-generated from PlayerController.java
// Target: Xbox 360 / C++03 / DirectX 9

class EntityPlayer;
class Minecraft;
class World;

class PlayerController {
public:
	PlayerController();
	PlayerController(Minecraft* var1);
	virtual ~PlayerController();

	bool m_isInTestMode;

	virtual void onWorldChange(World* var1);
	virtual void clickBlock(int var1, int var2, int var3);
	virtual bool sendBlockRemoved(int var1, int var2, int var3);
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void resetBlockRemoving();
	virtual void setPartialTime(float var1);
	virtual float getBlockReachDistance();
	virtual void onUpdate();
	virtual bool shouldDrawHUD();
	virtual void onRespawn(EntityPlayer* var1);

protected:
	Minecraft* mc;

};

#endif // PLAYERCONTROLLER_H