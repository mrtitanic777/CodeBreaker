#ifndef ENTITYCLIENTPLAYERMP_H
#define ENTITYCLIENTPLAYERMP_H

// Auto-generated from EntityClientPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityPlayerSP.h"

class Entity;
class EntityItem;
class InventoryPlayer;
class Minecraft;
class NetClientHandler;
class Session;
class World;

class EntityClientPlayerMP : public EntityPlayerSP {
public:
	EntityClientPlayerMP(Minecraft* var1, World* var2, Session* var3, NetClientHandler* var4);
	virtual ~EntityClientPlayerMP();

	NetClientHandler* sendQueue;
	int inventorySyncTimer;
	double oldPosX;
	double oldMinY;
	double oldPosY;
	double oldPosZ;
	float oldRotationYaw;
	float oldRotationPitch;
	InventoryPlayer* lastSentInventory;
	bool wasOnGround;
	bool wasSwinging;
	int positionUpdateTicks;

	virtual bool canAttackEntity(Entity* var1, int var2);
	virtual void heal(int var1);
	virtual void onUpdate();
	virtual void onUpdateClientPlayer();
	virtual void onUpdateWalkingPlayer();
	virtual void sendInventoryChanged();
	virtual void joinEntityItemWithWorld(EntityItem* var1);
	virtual void sendChatMessage(const JavaString& var1);
	virtual void swingItem();
	virtual void respawnPlayer();
	virtual void damageEntity(int var1);

	static void initStatics();
};

#endif // ENTITYCLIENTPLAYERMP_H