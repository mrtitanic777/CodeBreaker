#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet59ComplexEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet59ComplexEntity.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "CompressedStreamTools.h"
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
#include "IOException.h"
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

Packet59ComplexEntity::Packet59ComplexEntity()
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, entityNBT(NULL)
{
	this->isChunkDataPacket = true;
}

Packet59ComplexEntity::Packet59ComplexEntity(int var1, int var2, int var3, TileEntity* var4)
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, entityNBT(NULL)
{
	this->isChunkDataPacket = true;
	this->xPosition = var1;
	this->yPosition = var2;
	this->zPosition = var3;
	this->entityNBT = new NBTTagCompound();
	var4->writeToNBT(this->entityNBT);
	try {
	this->entityData = CompressedStreamTools::compress(this->entityNBT);
	} catch (IOException* var6) {
	var6->printStackTrace();
	}
}

Packet59ComplexEntity::~Packet59ComplexEntity()
{
	delete entityNBT;
	entityNBT = NULL;
}

void Packet59ComplexEntity::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readInt();
	this->yPosition = var1->readShort();
	this->zPosition = var1->readInt();
	int var2 = var1->readShort() & 0xffff;
	this->entityData = JArray<unsigned char >(var2);
	var1->readFully(this->entityData);
	this->entityNBT = CompressedStreamTools::decompress(this->entityData);
}

void Packet59ComplexEntity::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->xPosition);
	var1->writeShort(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->writeShort((short)this->entityData.length);
	var1->write(this->entityData);
}

void Packet59ComplexEntity::processPacket(NetHandler* var1)
{
	var1->handleComplexEntity(this);
}

int Packet59ComplexEntity::getPacketSize()
{
	return this->entityData.length + 2 + 10;
}

void Packet59ComplexEntity::initStatics()
{
}
