#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet20NamedEntitySpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet20NamedEntitySpawn.h"
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

Packet20NamedEntitySpawn::Packet20NamedEntitySpawn()
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, rotation(0)
	, pitch(0)
	, currentItem(0)
{
}

Packet20NamedEntitySpawn::Packet20NamedEntitySpawn(EntityPlayer* var1)
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, rotation(0)
	, pitch(0)
	, currentItem(0)
{
	this->entityId = var1->entityId;
	this->name = var1->field_771_i;
	this->xPosition = MathHelper::floor_double(var1->posX * 32.0);
	this->yPosition = MathHelper::floor_double(var1->posY * 32.0);
	this->zPosition = MathHelper::floor_double(var1->posZ * 32.0);
	this->rotation = (unsigned char)((int)(var1->rotationYaw * 256.0F / 360.0F));
	this->pitch = (unsigned char)((int)(var1->rotationPitch * 256.0F / 360.0F));
	ItemStack* var2 = var1->inventory->getCurrentItem();
	this->currentItem = var2 == NULL ? 0 : var2->itemID;
}

Packet20NamedEntitySpawn::~Packet20NamedEntitySpawn()
{
}

void Packet20NamedEntitySpawn::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->name = var1->readUTF();
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->zPosition = var1->readInt();
	this->rotation = var1->readByte();
	this->pitch = var1->readByte();
	this->currentItem = var1->readShort();
}

void Packet20NamedEntitySpawn::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeUTF(this->name);
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->writeByte(this->rotation);
	var1->writeByte(this->pitch);
	var1->writeShort(this->currentItem);
}

void Packet20NamedEntitySpawn::processPacket(NetHandler* var1)
{
	var1->handleNamedEntitySpawn(this);
}

int Packet20NamedEntitySpawn::getPacketSize()
{
	return 28;
}

void Packet20NamedEntitySpawn::initStatics()
{
}
