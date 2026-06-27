#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet21PickupSpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet21PickupSpawn.h"
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

Packet21PickupSpawn::Packet21PickupSpawn()
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, rotation(0)
	, pitch(0)
	, roll(0)
	, itemId(0)
	, count(0)
{
}

Packet21PickupSpawn::Packet21PickupSpawn(EntityItem* var1)
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, rotation(0)
	, pitch(0)
	, roll(0)
	, itemId(0)
	, count(0)
{
	this->entityId = var1->entityId;
	this->itemId = var1->item->itemID;
	this->count = var1->item->stackSize;
	this->xPosition = MathHelper::floor_double(var1->posX * 32.0);
	this->yPosition = MathHelper::floor_double(var1->posY * 32.0);
	this->zPosition = MathHelper::floor_double(var1->posZ * 32.0);
	this->rotation = (unsigned char)((int)(var1->motionX * 128.0));
	this->pitch = (unsigned char)((int)(var1->motionY * 128.0));
	this->roll = (unsigned char)((int)(var1->motionZ * 128.0));
}

Packet21PickupSpawn::~Packet21PickupSpawn()
{
}

void Packet21PickupSpawn::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->itemId = var1->readShort();
	this->count = var1->readByte();
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->zPosition = var1->readInt();
	this->rotation = var1->readByte();
	this->pitch = var1->readByte();
	this->roll = var1->readByte();
}

void Packet21PickupSpawn::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeShort(this->itemId);
	var1->writeByte(this->count);
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->writeByte(this->rotation);
	var1->writeByte(this->pitch);
	var1->writeByte(this->roll);
}

void Packet21PickupSpawn::processPacket(NetHandler* var1)
{
	var1->handlePickupSpawn(this);
}

int Packet21PickupSpawn::getPacketSize()
{
	return 22;
}

void Packet21PickupSpawn::initStatics()
{
}
