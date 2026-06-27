#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet34EntityTeleport.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet34EntityTeleport.h"
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
#include "MathHelper.h"
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

Packet34EntityTeleport::Packet34EntityTeleport()
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, yaw(0)
	, pitch(0)
{
}

Packet34EntityTeleport::Packet34EntityTeleport(Entity* var1)
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, yaw(0)
	, pitch(0)
{
	this->entityId = var1->entityId;
	this->xPosition = MathHelper::floor_double(var1->posX * 32.0);
	this->yPosition = MathHelper::floor_double(var1->posY * 32.0);
	this->zPosition = MathHelper::floor_double(var1->posZ * 32.0);
	this->yaw = (unsigned char)((int)(var1->rotationYaw * 256.0F / 360.0F));
	this->pitch = (unsigned char)((int)(var1->rotationPitch * 256.0F / 360.0F));
}

Packet34EntityTeleport::~Packet34EntityTeleport()
{
}

void Packet34EntityTeleport::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->zPosition = var1->readInt();
	this->yaw = (unsigned char)var1->read();
	this->pitch = (unsigned char)var1->read();
}

void Packet34EntityTeleport::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->write(this->yaw);
	var1->write(this->pitch);
}

void Packet34EntityTeleport::processPacket(NetHandler* var1)
{
	var1->handleEntityTeleport(this);
}

int Packet34EntityTeleport::getPacketSize()
{
	return 34;
}

void Packet34EntityTeleport::initStatics()
{
}
