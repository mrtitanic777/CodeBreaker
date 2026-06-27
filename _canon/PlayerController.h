#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

// Auto-generated from PlayerController.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Entity;
class EntityPlayer;
class ItemStack;
class Minecraft;
class World;

class PlayerController {
public:
	PlayerController(Minecraft* var1);
	virtual ~PlayerController();

	Minecraft* mc;
	bool isCreativeMode;

	virtual void addPendingConnection(World* var1);
	virtual void clickBlock(int var1, int var2, int var3, int var4);
	virtual bool sendBlockRemoved(int var1, int var2, int var3, int var4);
	virtual void sendBlockRemoving(int var1, int var2, int var3, int var4);
	virtual void resetBlockRemoving();
	virtual void updateBlockHitOverlay(float var1);
	virtual float getBlockReachDistance();
	virtual bool sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3);
	virtual void flipPlayer(EntityPlayer* var1);
	virtual void resetBlockRemovingSound();
	virtual bool shouldDrawHealthBar();
	virtual void onPlayerCreated(EntityPlayer* var1);
	virtual bool sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7);
	virtual EntityPlayer* createPlayer(World* var1);
	virtual void interactWithEntity(EntityPlayer* var1, Entity* var2);
	virtual void attackEntity(EntityPlayer* var1, Entity* var2);

	static void initStatics();
};

#endif // PLAYERCONTROLLER_H