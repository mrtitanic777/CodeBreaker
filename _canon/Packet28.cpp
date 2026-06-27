#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet28.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet28.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NetHandler.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "Packet.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

Packet28::Packet28()
	: entityId(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
}

Packet28::Packet28(Entity* var1)
	: entityId(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
	// inlined this(var1->entityId, var1->motionX, var1->motionY, var1->motionZ) -- C++03 lacks ctor delegation
	this->entityId = var1->entityId;
	double var8 = 3.9;
	if(var1->motionX < -var8) {
	var1->motionX = -var8;
	}
	if(var1->motionY < -var8) {
	var1->motionY = -var8;
	}
	if(var1->motionZ < -var8) {
	var1->motionZ = -var8;
	}
	if(var1->motionX > var8) {
	var1->motionX = var8;
	}
	if(var1->motionY > var8) {
	var1->motionY = var8;
	}
	if(var1->motionZ > var8) {
	var1->motionZ = var8;
	}
	this->velocityX = (int)(var1->motionX * 8000.0);
	this->velocityY = (int)(var1->motionY * 8000.0);
	this->velocityZ = (int)(var1->motionZ * 8000.0);
}

Packet28::Packet28(int var1, double var2, double var4, double var6)
	: entityId(0)
	, velocityX(0)
	, velocityY(0)
	, velocityZ(0)
{
	this->entityId = var1;
	double var8 = 3.9;
	if(var2 < -var8) {
	var2 = -var8;
	}
	if(var4 < -var8) {
	var4 = -var8;
	}
	if(var6 < -var8) {
	var6 = -var8;
	}
	if(var2 > var8) {
	var2 = var8;
	}
	if(var4 > var8) {
	var4 = var8;
	}
	if(var6 > var8) {
	var6 = var8;
	}
	this->velocityX = (int)(var2 * 8000.0);
	this->velocityY = (int)(var4 * 8000.0);
	this->velocityZ = (int)(var6 * 8000.0);
}

Packet28::~Packet28()
{
}

void Packet28::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->velocityX = var1->readShort();
	this->velocityY = var1->readShort();
	this->velocityZ = var1->readShort();
}

void Packet28::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeShort(this->velocityX);
	var1->writeShort(this->velocityY);
	var1->writeShort(this->velocityZ);
}

void Packet28::processPacket(NetHandler* var1)
{
	var1->handleEntityVelocity(this);
}

int Packet28::getPacketSize()
{
	return 10;
}

void Packet28::initStatics()
{
}
