#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet24MobSpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet24MobSpawn.h"
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
#include "EntityList.h"
#include "EntityLiving.h"
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

Packet24MobSpawn::Packet24MobSpawn()
	: entityId(0)
	, type(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, yaw(0)
	, pitch(0)
{
}

Packet24MobSpawn::Packet24MobSpawn(EntityLiving* var1)
	: entityId(0)
	, type(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, yaw(0)
	, pitch(0)
{
	this->entityId = var1->entityId;
	this->type = (unsigned char)EntityList::getEntityID(var1);
	this->xPosition = MathHelper::floor_double(var1->posX * 32.0);
	this->yPosition = MathHelper::floor_double(var1->posY * 32.0);
	this->zPosition = MathHelper::floor_double(var1->posZ * 32.0);
	this->yaw = (unsigned char)((int)(var1->rotationYaw * 256.0F / 360.0F));
	this->pitch = (unsigned char)((int)(var1->rotationPitch * 256.0F / 360.0F));
}

Packet24MobSpawn::~Packet24MobSpawn()
{
}

void Packet24MobSpawn::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->type = var1->readByte();
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->zPosition = var1->readInt();
	this->yaw = var1->readByte();
	this->pitch = var1->readByte();
}

void Packet24MobSpawn::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeByte(this->type);
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->writeByte(this->yaw);
	var1->writeByte(this->pitch);
}

void Packet24MobSpawn::processPacket(NetHandler* var1)
{
	var1->handleMobSpawn(this);
}

int Packet24MobSpawn::getPacketSize()
{
	return 19;
}

void Packet24MobSpawn::initStatics()
{
}
