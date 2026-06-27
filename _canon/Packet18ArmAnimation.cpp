#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet18ArmAnimation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet18ArmAnimation.h"
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

Packet18ArmAnimation::Packet18ArmAnimation()
	: entityId(0)
	, animate(0)
{
}

Packet18ArmAnimation::Packet18ArmAnimation(Entity* var1, int var2)
	: entityId(0)
	, animate(0)
{
	this->entityId = var1->entityId;
	this->animate = var2;
}

Packet18ArmAnimation::~Packet18ArmAnimation()
{
}

void Packet18ArmAnimation::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->animate = var1->readByte();
}

void Packet18ArmAnimation::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeByte(this->animate);
}

void Packet18ArmAnimation::processPacket(NetHandler* var1)
{
	var1->handleArmAnimation(this);
}

int Packet18ArmAnimation::getPacketSize()
{
	return 5;
}

void Packet18ArmAnimation::initStatics()
{
}
