#ifndef NETCLIENTHANDLER_H
#define NETCLIENTHANDLER_H

// Auto-generated from NetClientHandler.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "NetHandler.h"

class Entity;
class Minecraft;
class NetworkManager;
class Packet;
class Packet10Flying;
class Packet16BlockItemSwitch;
class Packet17AddToInventory;
class Packet18ArmAnimation;
class Packet1Login;
class Packet20NamedEntitySpawn;
class Packet21PickupSpawn;
class Packet22Collect;
class Packet23VehicleSpawn;
class Packet24MobSpawn;
class Packet255KickDisconnect;
class Packet28;
class Packet29DestroyEntity;
class Packet2Handshake;
class Packet30Entity;
class Packet34EntityTeleport;
class Packet38;
class Packet39;
class Packet3Chat;
class Packet4UpdateTime;
class Packet50PreChunk;
class Packet51MapChunk;
class Packet52MultiBlockChange;
class Packet53BlockChange;
class Packet59ComplexEntity;
class Packet5PlayerInventory;
class Packet60;
class Packet6SpawnPosition;
class Packet8;
class Packet9;
class Random;
class WorldClient;

class NetClientHandler : public NetHandler {
public:
	NetClientHandler(Minecraft* var1, const JavaString& var2, int var3);
	virtual ~NetClientHandler();

	bool disconnected;
	NetworkManager* netManager;
	JavaString connectionStatus;
	Minecraft* mc;
	WorldClient* worldClient;
	bool receivedFirstUpdate;
	Random* rand;

	virtual void processReadPackets();
	virtual void handleLogin(Packet1Login* var1);
	virtual void handlePickupSpawn(Packet21PickupSpawn* var1);
	virtual void handleVehicleSpawn(Packet23VehicleSpawn* var1);
	virtual void handleEntityVelocity(Packet28* var1);
	virtual void handleNamedEntitySpawn(Packet20NamedEntitySpawn* var1);
	virtual void handleEntityTeleport(Packet34EntityTeleport* var1);
	virtual void handleEntity(Packet30Entity* var1);
	virtual void handleDestroyEntity(Packet29DestroyEntity* var1);
	virtual void handleFlying(Packet10Flying* var1);
	virtual void handlePreChunk(Packet50PreChunk* var1);
	virtual void handleMultiBlockChange(Packet52MultiBlockChange* var1);
	virtual void handleMapChunk(Packet51MapChunk* var1);
	virtual void handleBlockChange(Packet53BlockChange* var1);
	virtual void handleKickDisconnect(Packet255KickDisconnect* var1);
	virtual void handleErrorMessage(const JavaString& var1);
	virtual void addToSendQueue(Packet* var1);
	virtual void handleCollect(Packet22Collect* var1);
	virtual void handleBlockItemSwitch(Packet16BlockItemSwitch* var1);
	virtual void handleChat(Packet3Chat* var1);
	virtual void handleArmAnimation(Packet18ArmAnimation* var1);
	virtual void handleAddToInventory(Packet17AddToInventory* var1);
	virtual void handleHandshake(Packet2Handshake* var1);
	virtual void disconnect();
	virtual void handleMobSpawn(Packet24MobSpawn* var1);
	virtual void handleUpdateTime(Packet4UpdateTime* var1);
	virtual void handlePlayerInventory(Packet5PlayerInventory* var1);
	virtual void handleComplexEntity(Packet59ComplexEntity* var1);
	virtual void handleSpawnPosition(Packet6SpawnPosition* var1);
	virtual void handleAttachEntity(Packet39* var1);
	virtual void handleEntityStatus(Packet38* var1);
	virtual Entity* getEntityByID(int var1);
	virtual void handleHealth(Packet8* var1);
	virtual void handleRespawn(Packet9* var1);
	virtual void handleExplosion(Packet60* var1);

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	NetClientHandler(const NetClientHandler&);
	NetClientHandler& operator=(const NetClientHandler&);
};

#endif // NETCLIENTHANDLER_H