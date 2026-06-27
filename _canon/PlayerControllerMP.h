#ifndef PLAYERCONTROLLERMP_H
#define PLAYERCONTROLLERMP_H

// Auto-generated from PlayerControllerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "PlayerController.h"

class Entity;
class EntityPlayer;
class ItemStack;
class Minecraft;
class NetClientHandler;
class World;

class PlayerControllerMP : public PlayerController {
public:
	PlayerControllerMP(Minecraft* var1, NetClientHandler* var2);
	virtual ~PlayerControllerMP();

	int currentBlockX;
	int currentBlockY;
	int currentBlockZ;
	float curBlockDamageMP;
	float prevBlockDamageMP;
	float stepSoundTickCounter;
	int blockHitDelay;
	bool isHittingBlock;
	NetClientHandler* netClientHandler;
	int currentPlayerItem;

	virtual void flipPlayer(EntityPlayer* var1);
	virtual bool sendBlockRemoved(int var1, int var2, int var3, int var4);
	virtual void clickBlock(int var1, int var2, int var3, int var4);
	virtual void resetBlockRemoving();
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void updateBlockHitOverlay(float var1);
	virtual float getBlockReachDistance();
	virtual void addPendingConnection(World* var1);
	virtual void resetBlockRemovingSound();
	virtual void syncCurrentPlayItem();
	virtual bool sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7);
	virtual bool sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3);
	virtual EntityPlayer* createPlayer(World* var1);
	virtual void attackEntity(EntityPlayer* var1, Entity* var2);
	virtual void interactWithEntity(EntityPlayer* var1, Entity* var2);

	static void initStatics();
};

#endif // PLAYERCONTROLLERMP_H