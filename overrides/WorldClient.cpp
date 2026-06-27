#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldClient.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "BufferedImage.h"
#include "ByteBuffer.h"
#include "Canvas.h"
#include "Chunk.h"
#include "ChunkProviderClient.h"
#include "Class.h"
#include "Component.h"
#include "Cursor.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "EffectRenderer.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "EntityRenderer.h"
#include "EnumOS2.h"
#include "Explosion.h"
#include "File.h"
#include "FloatBuffer.h"
#include "FontRenderer.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiIngame.h"
#include "GuiScreen.h"
#include "HashEntry.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IWorldAccess.h"
#include "InputStream.h"
#include "IntBuffer.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemRenderer.h"
#include "ItemStack.h"
#include "KeyBinding.h"
#include "LoadingScreenRenderer.h"
#include "MCHashTable.h"
#include "Material.h"
#include "Minecraft.h"
#include "MinecraftApplet.h"
#include "MobSpawnerBase.h"
#include "ModelBase.h"
#include "ModelBiped.h"
#include "ModelRenderer.h"
#include "MouseHelper.h"
#include "MovementInput.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NetClientHandler.h"
#include "NetHandler.h"
#include "NetworkManager.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "OpenGlCapsChecker.h"
#include "Packet.h"
#include "Packet255KickDisconnect.h"
#include "Packet59ComplexEntity.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "PlayerController.h"
#include "PositionTexureVertex.h"
#include "Random.h"
#include "Render.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderGlobal.h"
#include "RenderItem.h"
#include "RenderList.h"
#include "RenderManager.h"
#include "Session.h"
#include "Socket.h"
#include "SoundManager.h"
#include "SoundPool.h"
#include "SoundPoolEntry.h"
#include "SoundSystem.h"
#include "StepSound.h"
#include "Tessellator.h"
#include "TextureLavaFX.h"
#include "TexturePackBase.h"
#include "TexturePackList.h"
#include "TextureWaterFX.h"
#include "TexturedQuad.h"
#include "Thread.h"
#include "ThreadDownloadImageData.h"
#include "ThreadDownloadResources.h"
#include "TileEntity.h"
#include "Timer.h"
#include "URL.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldBlockPositionType.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"

WorldClient::WorldClient(NetClientHandler* var1, long long var2, int var4)
	: World("MpServer", WorldProvider::getProviderForDimension(var4), var2)
	, sendQueue(NULL)
	, C(NULL)
	, D(0)
	, entitiesById(NULL)
{
	this->entitiesById = new MCHashTable();

	this->sendQueue = var1;
	this->spawnX = 8;
	this->spawnY = 64;
	this->spawnZ = 8;
}

WorldClient::~WorldClient()
{
}

void WorldClient::tick()
{
	++this->worldTime;
	int var1 = this->calculateSkylightSubtracted(1.0F);
	int var2;
	if(var1 != this->skylightSubtracted) {
	this->skylightSubtracted = var1;
	for(var2 = 0; var2 < this->worldAccesses.size(); ++var2) {
	((IWorldAccess*)this->worldAccesses[var2])->updateAllRenderers();
	}
	}
	for(var2 = 0; var2 < 10 && !this->entitySpawnQueue.empty(); ++var2) {
	Entity* var3 = (Entity*)*this->entitySpawnQueue.begin();
	if(!this->loadedEntityList.contains(var3)) {
	this->entityJoinedWorld(var3);
	}
	}
	this->sendQueue->processReadPackets();
	for(var2 = 0; var2 < this->blockChangeList.size(); ++var2) {
	WorldBlockPositionType* var4 = (WorldBlockPositionType*)this->blockChangeList[var2];
	if(--var4->countdown == 0) {
	World::setBlockAndMetadata(var4->xCoord, var4->yCoord, var4->zCoord, var4->blockId, var4->metadata);
	World::markBlockNeedsUpdate(var4->xCoord, var4->yCoord, var4->zCoord);
	this->blockChangeList.remove(var2--);
	}
	}
}

void WorldClient::invalidateBlockReceiveRegion(int var1, int var2, int var3, int var4, int var5, int var6)
{
	for(int var7 = 0; var7 < this->blockChangeList.size(); ++var7) {
	WorldBlockPositionType* var8 = (WorldBlockPositionType*)this->blockChangeList[var7];
	if(var8->xCoord >= var1 && var8->yCoord >= var2 && var8->zCoord >= var3 && var8->xCoord <= var4 && var8->yCoord <= var5 && var8->zCoord <= var6) {
	this->blockChangeList.remove(var7--);
	}
	}
}

IChunkProvider* WorldClient::createChunkProvider(File* var1)
{
	this->C = new ChunkProviderClient(this);
	return this->C;
}

void WorldClient::findSpawnGround()
{
	this->spawnX = 8;
	this->spawnY = 64;
	this->spawnZ = 8;
}

void WorldClient::updateBlocksAroundPlayers()
{
}

void WorldClient::scheduleBlockUpdate(int var1, int var2, int var3, int var4)
{
}

bool WorldClient::TickUpdates(bool var1)
{
	return false;
}

void WorldClient::getServerSocket(int var1, int var2, bool var3)
{
	if(var3) {
	this->C->loadChunk(var1, var2);
	} else {
	this->C->unloadChunk(var1, var2);
	}
	if(!var3) {
	this->markBlockRangeNeedsUpdate(var1 * 16, 0, var2 * 16, var1 * 16 + 15, 128, var2 * 16 + 15);
	}
}

bool WorldClient::entityJoinedWorld(Entity* var1)
{
	bool var2 = World::entityJoinedWorld(var1);
	this->F.insert(var1);
	if(!var2) {
	this->entitySpawnQueue.insert(var1);
	}
	return var2;
}

void WorldClient::setEntityDead(Entity* var1)
{
	World::setEntityDead(var1);
	this->F.erase(var1);
}

void WorldClient::obtainEntitySkin(Entity* var1)
{
	World::obtainEntitySkin(var1);
	if(this->entitySpawnQueue.count(var1)) {
	this->entitySpawnQueue.erase(var1);
	}
}

void WorldClient::releaseEntitySkin(Entity* var1)
{
	World::releaseEntitySkin(var1);
	if(this->F.count(var1)) {
	this->entitySpawnQueue.insert(var1);
	}
}

void WorldClient::addEntityToWorld(int var1, Entity* var2)
{
	Entity* var3 = this->getEntityByID(var1);
	if(var3 != NULL) {
	this->setEntityDead(var3);
	}
	this->F.insert(var2);
	var2->entityId = var1;
	if(!this->entityJoinedWorld(var2)) {
	this->entitySpawnQueue.insert(var2);
	}
	this->entitiesById->addKey(var1, var2);
}

Entity* WorldClient::getEntityByID(int var1)
{
	return (Entity*)this->entitiesById->lookup(var1);
}

Entity* WorldClient::removeEntityFromWorld(int var1)
{
	Entity* var2 = (Entity*)this->entitiesById->removeObject(var1);
	if(var2 != NULL) {
	this->F.erase(var2);
	this->setEntityDead(var2);
	}
	return var2;
}

bool WorldClient::setBlockMetadata(int var1, int var2, int var3, int var4)
{
	int var5 = this->getBlockId(var1, var2, var3);
	int var6 = this->getBlockMetadata(var1, var2, var3);
	if(World::setBlockMetadata(var1, var2, var3, var4)) {
	this->blockChangeList.push_back(new WorldBlockPositionType(this, var1, var2, var3, var5, var6));
	return true;
	} else {
	return false;
	}
}

bool WorldClient::setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5)
{
	int var6 = this->getBlockId(var1, var2, var3);
	int var7 = this->getBlockMetadata(var1, var2, var3);
	if(World::setBlockAndMetadata(var1, var2, var3, var4, var5)) {
	this->blockChangeList.push_back(new WorldBlockPositionType(this, var1, var2, var3, var6, var7));
	return true;
	} else {
	return false;
	}
}

bool WorldClient::setBlock(int var1, int var2, int var3, int var4)
{
	int var5 = this->getBlockId(var1, var2, var3);
	int var6 = this->getBlockMetadata(var1, var2, var3);
	if(World::setBlock(var1, var2, var3, var4)) {
	this->blockChangeList.push_back(new WorldBlockPositionType(this, var1, var2, var3, var5, var6));
	return true;
	} else {
	return false;
	}
}

bool WorldClient::setBlockAndMetadataNoQueue(int var1, int var2, int var3, int var4, int var5)
{
	this->invalidateBlockReceiveRegion(var1, var2, var3, var1, var2, var3);
	if(World::setBlockAndMetadata(var1, var2, var3, var4, var5)) {
	this->notifyBlockChange(var1, var2, var3, var4);
	return true;
	} else {
	return false;
	}
}

void WorldClient::markTileEntityForUpdate(int var1, int var2, int var3, TileEntity* var4)
{
	if(!this->D) {
	this->sendQueue->addToSendQueue(new Packet59ComplexEntity(var1, var2, var3, var4));
	}
}

void WorldClient::sendQuittingDisconnectingPacket()
{
	this->sendQueue->addToSendQueue(new Packet255KickDisconnect("Quitting"));
}
