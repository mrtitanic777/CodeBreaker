#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MinecartTrackLogic.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MinecartTrackLogic.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockMinecartTrack.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkPosition.h"
#include "Class.h"
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
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

MinecartTrackLogic::MinecartTrackLogic(BlockMinecartTrack* var1, World* var2, int var3, int var4, int var5)
	: worldObj(NULL)
	, trackX(0)
	, trackY(0)
	, trackZ(0)
	, trackMetadata(0)
	, theTrack(NULL)
{
	this->theTrack = var1;
	this->connectingTracks = JList<void*>();
	this->worldObj = var2;
	this->trackX = var3;
	this->trackY = var4;
	this->trackZ = var5;
	this->trackMetadata = var2->getBlockMetadata(var3, var4, var5);
	this->setConnections();
}

MinecartTrackLogic::~MinecartTrackLogic()
{
}

void MinecartTrackLogic::setConnections()
{
	this->connectingTracks.clear();
	if(this->trackMetadata == 0) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ - 1));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ + 1));
	} else if(this->trackMetadata == 1) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX - 1, this->trackY, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX + 1, this->trackY, this->trackZ));
	} else if(this->trackMetadata == 2) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX - 1, this->trackY, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX + 1, this->trackY + 1, this->trackZ));
	} else if(this->trackMetadata == 3) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX - 1, this->trackY + 1, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX + 1, this->trackY, this->trackZ));
	} else if(this->trackMetadata == 4) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY + 1, this->trackZ - 1));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ + 1));
	} else if(this->trackMetadata == 5) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ - 1));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY + 1, this->trackZ + 1));
	} else if(this->trackMetadata == 6) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX + 1, this->trackY, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ + 1));
	} else if(this->trackMetadata == 7) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX - 1, this->trackY, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ + 1));
	} else if(this->trackMetadata == 8) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX - 1, this->trackY, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ - 1));
	} else if(this->trackMetadata == 9) {
	this->connectingTracks.push_back(new ChunkPosition(this->trackX + 1, this->trackY, this->trackZ));
	this->connectingTracks.push_back(new ChunkPosition(this->trackX, this->trackY, this->trackZ - 1));
	}
}

void MinecartTrackLogic::refreshConnectedTracks()
{
	for(int var1 = 0; var1 < this->connectingTracks.size(); ++var1) {
	MinecartTrackLogic* var2 = this->getRailLogic((ChunkPosition*)this->connectingTracks[var1]);
	if(var2 != NULL && var2->isConnectedToTrack(this)) {
	this->connectingTracks.set(var1, new ChunkPosition(var2->trackX, var2->trackY, var2->trackZ));
	} else {
	this->connectingTracks.remove(var1--);
	}
	}
}

bool MinecartTrackLogic::isMinecartTrack(int var1, int var2, int var3)
{
	return this->worldObj->getBlockId(var1, var2, var3) == this->theTrack->blockID ? true : (this->worldObj->getBlockId(var1, var2 + 1, var3) == this->theTrack->blockID ? true : this->worldObj->getBlockId(var1, var2 - 1, var3) == this->theTrack->blockID);
}

MinecartTrackLogic* MinecartTrackLogic::getRailLogic(ChunkPosition* var1)
{
	return this->worldObj->getBlockId(var1->x, var1->y, var1->z) == this->theTrack->blockID ? new MinecartTrackLogic(this->theTrack, this->worldObj, var1->x, var1->y, var1->z) : (this->worldObj->getBlockId(var1->x, var1->y + 1, var1->z) == this->theTrack->blockID ? new MinecartTrackLogic(this->theTrack, this->worldObj, var1->x, var1->y + 1, var1->z) : (this->worldObj->getBlockId(var1->x, var1->y - 1, var1->z) == this->theTrack->blockID ? new MinecartTrackLogic(this->theTrack, this->worldObj, var1->x, var1->y - 1, var1->z) : NULL));
}

bool MinecartTrackLogic::isConnectedToTrack(MinecartTrackLogic* var1)
{
	for(int var2 = 0; var2 < this->connectingTracks.size(); ++var2) {
	ChunkPosition* var3 = (ChunkPosition*)this->connectingTracks[var2];
	if(var3->x == var1->trackX && var3->z == var1->trackZ) {
	return true;
	}
	}
	return false;
}

bool MinecartTrackLogic::hasConnection(int var1, int var2, int var3)
{
	for(int var4 = 0; var4 < this->connectingTracks.size(); ++var4) {
	ChunkPosition* var5 = (ChunkPosition*)this->connectingTracks[var4];
	if(var5->x == var1 && var5->z == var3) {
	return true;
	}
	}
	return false;
}

int MinecartTrackLogic::getAdjacentTracks()
{
	int var1 = 0;
	if(this->isMinecartTrack(this->trackX, this->trackY, this->trackZ - 1)) {
	++var1;
	}
	if(this->isMinecartTrack(this->trackX, this->trackY, this->trackZ + 1)) {
	++var1;
	}
	if(this->isMinecartTrack(this->trackX - 1, this->trackY, this->trackZ)) {
	++var1;
	}
	if(this->isMinecartTrack(this->trackX + 1, this->trackY, this->trackZ)) {
	++var1;
	}
	return var1;
}

bool MinecartTrackLogic::canConnectTo(MinecartTrackLogic* var1)
{
	if(this->isConnectedToTrack(var1)) {
	return true;
	} else if(this->connectingTracks.size() == 2) {
	return false;
	} else if(this->connectingTracks.size() == 0) {
	return true;
	} else {
	ChunkPosition* var2 = (ChunkPosition*)this->connectingTracks[0];
	return var1->trackY == this->trackY && var2->y == this->trackY ? true : true;
	}
}

void MinecartTrackLogic::connectTo(MinecartTrackLogic* var1)
{
	this->connectingTracks.push_back(new ChunkPosition(var1->trackX, var1->trackY, var1->trackZ));
	bool var2 = this->hasConnection(this->trackX, this->trackY, this->trackZ - 1);
	bool var3 = this->hasConnection(this->trackX, this->trackY, this->trackZ + 1);
	bool var4 = this->hasConnection(this->trackX - 1, this->trackY, this->trackZ);
	bool var5 = this->hasConnection(this->trackX + 1, this->trackY, this->trackZ);
	unsigned char var6 = -1;
	if(var2 || var3) {
	var6 = 0;
	}
	if(var4 || var5) {
	var6 = 1;
	}
	if(var3 && var5 && !var2 && !var4) {
	var6 = 6;
	}
	if(var3 && var4 && !var2 && !var5) {
	var6 = 7;
	}
	if(var2 && var4 && !var3 && !var5) {
	var6 = 8;
	}
	if(var2 && var5 && !var3 && !var4) {
	var6 = 9;
	}
	if(var6 == 0) {
	if(this->worldObj->getBlockId(this->trackX, this->trackY + 1, this->trackZ - 1) == this->theTrack->blockID) {
	var6 = 4;
	}
	if(this->worldObj->getBlockId(this->trackX, this->trackY + 1, this->trackZ + 1) == this->theTrack->blockID) {
	var6 = 5;
	}
	}
	if(var6 == 1) {
	if(this->worldObj->getBlockId(this->trackX + 1, this->trackY + 1, this->trackZ) == this->theTrack->blockID) {
	var6 = 2;
	}
	if(this->worldObj->getBlockId(this->trackX - 1, this->trackY + 1, this->trackZ) == this->theTrack->blockID) {
	var6 = 3;
	}
	}
	if(var6 < 0) {
	var6 = 0;
	}
	this->worldObj->setBlockMetadataWithNotify(this->trackX, this->trackY, this->trackZ, var6);
}

bool MinecartTrackLogic::canConnectFrom(int var1, int var2, int var3)
{
	MinecartTrackLogic* var4 = this->getRailLogic(new ChunkPosition(var1, var2, var3));
	if(var4 == NULL) {
	return false;
	} else {
	var4->refreshConnectedTracks();
	return var4->canConnectTo(this);
	}
}

void MinecartTrackLogic::refreshTrackShape(bool var1)
{
	bool var2 = this->canConnectFrom(this->trackX, this->trackY, this->trackZ - 1);
	bool var3 = this->canConnectFrom(this->trackX, this->trackY, this->trackZ + 1);
	bool var4 = this->canConnectFrom(this->trackX - 1, this->trackY, this->trackZ);
	bool var5 = this->canConnectFrom(this->trackX + 1, this->trackY, this->trackZ);
	unsigned char var6 = -1;
	if((var2 || var3) && !var4 && !var5) {
	var6 = 0;
	}
	if((var4 || var5) && !var2 && !var3) {
	var6 = 1;
	}
	if(var3 && var5 && !var2 && !var4) {
	var6 = 6;
	}
	if(var3 && var4 && !var2 && !var5) {
	var6 = 7;
	}
	if(var2 && var4 && !var3 && !var5) {
	var6 = 8;
	}
	if(var2 && var5 && !var3 && !var4) {
	var6 = 9;
	}
	if(var6 == -1) {
	if(var2 || var3) {
	var6 = 0;
	}
	if(var4 || var5) {
	var6 = 1;
	}
	if(var1) {
	if(var3 && var5) {
	var6 = 6;
	}
	if(var4 && var3) {
	var6 = 7;
	}
	if(var5 && var2) {
	var6 = 9;
	}
	if(var2 && var4) {
	var6 = 8;
	}
	} else {
	if(var2 && var4) {
	var6 = 8;
	}
	if(var5 && var2) {
	var6 = 9;
	}
	if(var4 && var3) {
	var6 = 7;
	}
	if(var3 && var5) {
	var6 = 6;
	}
	}
	}
	if(var6 == 0) {
	if(this->worldObj->getBlockId(this->trackX, this->trackY + 1, this->trackZ - 1) == this->theTrack->blockID) {
	var6 = 4;
	}
	if(this->worldObj->getBlockId(this->trackX, this->trackY + 1, this->trackZ + 1) == this->theTrack->blockID) {
	var6 = 5;
	}
	}
	if(var6 == 1) {
	if(this->worldObj->getBlockId(this->trackX + 1, this->trackY + 1, this->trackZ) == this->theTrack->blockID) {
	var6 = 2;
	}
	if(this->worldObj->getBlockId(this->trackX - 1, this->trackY + 1, this->trackZ) == this->theTrack->blockID) {
	var6 = 3;
	}
	}
	if(var6 < 0) {
	var6 = 0;
	}
	this->trackMetadata = var6;
	this->setConnections();
	this->worldObj->setBlockMetadataWithNotify(this->trackX, this->trackY, this->trackZ, var6);
	for(int var7 = 0; var7 < this->connectingTracks.size(); ++var7) {
	MinecartTrackLogic* var8 = this->getRailLogic((ChunkPosition*)this->connectingTracks[var7]);
	if(var8 != NULL) {
	var8->refreshConnectedTracks();
	if(var8->canConnectTo(this)) {
	var8->connectTo(this);
	}
	}
	}
}

int MinecartTrackLogic::getNumberOfAdjacentTracks(MinecartTrackLogic* var0)
{
	return var0->getAdjacentTracks();
}

void MinecartTrackLogic::initStatics()
{
}
