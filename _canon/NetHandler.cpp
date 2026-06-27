#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from NetHandler.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NetHandler.h"
#include "Item.h"
#include "ItemStack.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "Packet.h"
#include "Packet10Flying.h"
#include "Packet14BlockDig.h"
#include "Packet15Place.h"
#include "Packet16BlockItemSwitch.h"
#include "Packet17AddToInventory.h"
#include "Packet18ArmAnimation.h"
#include "Packet1Login.h"
#include "Packet20NamedEntitySpawn.h"
#include "Packet21PickupSpawn.h"
#include "Packet22Collect.h"
#include "Packet23VehicleSpawn.h"
#include "Packet24MobSpawn.h"
#include "Packet255KickDisconnect.h"
#include "Packet28.h"
#include "Packet29DestroyEntity.h"
#include "Packet2Handshake.h"
#include "Packet30Entity.h"
#include "Packet34EntityTeleport.h"
#include "Packet38.h"
#include "Packet39.h"
#include "Packet3Chat.h"
#include "Packet4UpdateTime.h"
#include "Packet50PreChunk.h"
#include "Packet51MapChunk.h"
#include "Packet52MultiBlockChange.h"
#include "Packet53BlockChange.h"
#include "Packet59ComplexEntity.h"
#include "Packet5PlayerInventory.h"
#include "Packet60.h"
#include "Packet6SpawnPosition.h"
#include "Packet7.h"
#include "Packet8.h"
#include "Packet9.h"
#include "Random.h"

NetHandler::~NetHandler()
{
}

void NetHandler::handleMapChunk(Packet51MapChunk* var1)
{
}

void NetHandler::unexpectedPacket(Packet* var1)
{
}

void NetHandler::handleErrorMessage(const JavaString& var1)
{
}

void NetHandler::handleKickDisconnect(Packet255KickDisconnect* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleLogin(Packet1Login* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleFlying(Packet10Flying* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleMultiBlockChange(Packet52MultiBlockChange* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleBlockDig(Packet14BlockDig* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleBlockChange(Packet53BlockChange* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handlePreChunk(Packet50PreChunk* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleNamedEntitySpawn(Packet20NamedEntitySpawn* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleEntity(Packet30Entity* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleEntityTeleport(Packet34EntityTeleport* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handlePlace(Packet15Place* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleBlockItemSwitch(Packet16BlockItemSwitch* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleDestroyEntity(Packet29DestroyEntity* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handlePickupSpawn(Packet21PickupSpawn* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleCollect(Packet22Collect* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleChat(Packet3Chat* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleAddToInventory(Packet17AddToInventory* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleVehicleSpawn(Packet23VehicleSpawn* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleArmAnimation(Packet18ArmAnimation* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleHandshake(Packet2Handshake* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleMobSpawn(Packet24MobSpawn* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleUpdateTime(Packet4UpdateTime* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handlePlayerInventory(Packet5PlayerInventory* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleComplexEntity(Packet59ComplexEntity* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleSpawnPosition(Packet6SpawnPosition* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleEntityVelocity(Packet28* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleAttachEntity(Packet39* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleUseEntity(Packet7* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleEntityStatus(Packet38* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleHealth(Packet8* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleRespawn(Packet9* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::handleExplosion(Packet60* var1)
{
	this->unexpectedPacket(var1);
}

void NetHandler::initStatics()
{
}
