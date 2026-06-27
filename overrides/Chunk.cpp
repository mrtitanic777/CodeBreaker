#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Chunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Chunk.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockContainer.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "ChunkPosition.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "EnumSkyBlock.h"
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
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "Thread.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

// Pack a ChunkPosition's (x,y,z) into the long long key used by chunkTileEntityMap,
// matching Java's value-equality on ChunkPosition (block coords are small/bounded).
namespace {
	inline long long _cpKey(ChunkPosition* p) {
		return ((long long)(p->x & 0xFFFF) << 32)
		     | ((long long)(p->y & 0xFFFF) << 16)
		     | ((long long)(p->z & 0xFFFF));
	}
}

bool Chunk::s_isLit = 0;

Chunk::Chunk(World* var1, int var2, int var3)
	: isChunkLoaded(0)
	, worldObj(NULL)
	, data(NULL)
	, skylightMap(NULL)
	, blocklightMap(NULL)
	, heightMapMinimum(0)
	, xPosition(0)
	, zPosition(0)
	, isTerrainPopulated(0)
	, isModified(0)
	, neverSave(0)
	, isEmptyChunk(0)
	, hasEntities(0)
	, lastSaveTime(0)
{
	this->chunkTileEntityMap = std::map<long long, void*>();
	this->entities = JArray<JList<void*> >(8);
	this->isTerrainPopulated = false;
	this->isModified = false;
	this->isEmptyChunk = false;
	this->hasEntities = false;
	this->lastSaveTime = 0LL;
	this->worldObj = var1;
	this->xPosition = var2;
	this->zPosition = var3;
	this->heightMap = JArray<unsigned char >(256);
	for(int var4 = 0; var4 < this->entities.length; ++var4) {
	this->entities[var4] = JList<void*>();
	}
}

Chunk::Chunk(World* var1, JArray<unsigned char > var2, int var3, int var4)
	: isChunkLoaded(0)
	, worldObj(NULL)
	, data(NULL)
	, skylightMap(NULL)
	, blocklightMap(NULL)
	, heightMapMinimum(0)
	, xPosition(0)
	, zPosition(0)
	, isTerrainPopulated(0)
	, isModified(0)
	, neverSave(0)
	, isEmptyChunk(0)
	, hasEntities(0)
	, lastSaveTime(0)
{
	// inlined this(var1, var3, var4) -- C++03 lacks ctor delegation
	this->chunkTileEntityMap = std::map<long long, void*>();
	this->entities = JArray<JList<void*> >(8);
	this->isTerrainPopulated = false;
	this->isModified = false;
	this->isEmptyChunk = false;
	this->hasEntities = false;
	this->lastSaveTime = 0LL;
	this->worldObj = var1;
	this->xPosition = var3;
	this->zPosition = var4;
	this->heightMap = JArray<unsigned char >(256);
	for(int var4 = 0; var4 < this->entities.length; ++var4) {
	this->entities[var4] = JList<void*>();
	}
	this->blocks = var2;
	this->data = new NibbleArray(var2.length);
	this->skylightMap = new NibbleArray(var2.length);
	this->blocklightMap = new NibbleArray(var2.length);
}

Chunk::~Chunk()
{
	delete blocklightMap;
	blocklightMap = NULL;
	delete data;
	data = NULL;
	delete skylightMap;
	skylightMap = NULL;
}

bool Chunk::isAtLocation(int var1, int var2)
{
	return var1 == this->xPosition && var2 == this->zPosition;
}

int Chunk::getHeightValue(int var1, int var2)
{
	return this->heightMap[var2 << 4 | var1] & 255;
}

void Chunk::removeUnknownBlocks()
{
}

void Chunk::generateHeightMap()
{
	int var1 = 127;
	for(int var2 = 0; var2 < 16; ++var2) {
	for(int var3 = 0; var3 < 16; ++var3) {
	int var4 = 127;
	for(int var5 = var2 << 11 | var3 << 7; var4 > 0 && Block::s_lightOpacity[this->blocks[var5 + var4 - 1]] == 0; --var4) {
	}
	this->heightMap[var3 << 4 | var2] = (unsigned char)var4;
	if(var4 < var1) {
	var1 = var4;
	}
	}
	}
	this->heightMapMinimum = var1;
	this->isModified = true;
}

void Chunk::generateSkylightMap()
{
	int var1 = 127;
	int var2;
	int var3;
	for(var2 = 0; var2 < 16; ++var2) {
	for(var3 = 0; var3 < 16; ++var3) {
	this->heightMap[var3 << 4 | var2] = -128;
	this->relightBlock(var2, 127, var3);
	if((this->heightMap[var3 << 4 | var2] & 255) < var1) {
	var1 = this->heightMap[var3 << 4 | var2] & 255;
	}
	}
	}
	this->heightMapMinimum = var1;
	for(var2 = 0; var2 < 16; ++var2) {
	for(var3 = 0; var3 < 16; ++var3) {
	this->checkSkylightNeighbors(var2, var3);
	}
	}
	this->isModified = true;
}

void Chunk::generateBlocklightMap()
{
	unsigned char var1 = 32;
	for(int var2 = 0; var2 < 16; ++var2) {
	for(int var3 = 0; var3 < 16; ++var3) {
	int var4 = (var2 << 11) | (var3 << 7);
	int var5;
	int var6;
	for(var5 = 0; var5 < 128; ++var5) {
	var6 = Block::s_lightValue[this->blocks[var4 + var5]];
	if(var6 > 0) {
	this->blocklightMap->setNibble(var2, var5, var3, var6);
	}
	}
	var5 = 15;
	for(var6 = var1 - 2; var6 < 128 && var5 > 0; this->blocklightMap->setNibble(var2, var6, var3, var5)) {
	++var6;
	unsigned char var7 = this->blocks[var4 + var6];
	int var8 = Block::s_lightOpacity[var7];
	int var9 = Block::s_lightValue[var7];
	if(var8 == 0) {
	var8 = 1;
	}
	var5 -= var8;
	if(var9 > var5) {
	var5 = var9;
	}
	if(var5 < 0) {
	var5 = 0;
	}
	}
	}
	}
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Block, this->xPosition * 16, var1 - 1, this->zPosition * 16, this->xPosition * 16 + 16, var1 + 1, this->zPosition * 16 + 16);
	this->isModified = true;
}

void Chunk::checkSkylightNeighbors(int var1, int var2)
{
	int var3 = this->getHeightValue(var1, var2);
	int var4 = this->xPosition * 16 + var1;
	int var5 = this->zPosition * 16 + var2;
	this->updateSkylightNeighborHeight(var4 - 1, var5, var3);
	this->updateSkylightNeighborHeight(var4 + 1, var5, var3);
	this->updateSkylightNeighborHeight(var4, var5 - 1, var3);
	this->updateSkylightNeighborHeight(var4, var5 + 1, var3);
}

void Chunk::updateSkylightNeighborHeight(int var1, int var2, int var3)
{
	int var4 = this->worldObj->getHeightValue(var1, var2);
	if(var4 > var3) {
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Sky, var1, var3, var2, var1, var4, var2);
	} else if(var4 < var3) {
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Sky, var1, var4, var2, var1, var3, var2);
	}
	this->isModified = true;
}

void Chunk::relightBlock(int var1, int var2, int var3)
{
	int var4 = this->heightMap[var3 << 4 | var1] & 255;
	int var5 = var4;
	if(var2 > var4) {
	var5 = var2;
	}
	for(int var6 = var1 << 11 | var3 << 7; var5 > 0 && Block::s_lightOpacity[this->blocks[var6 + var5 - 1]] == 0; --var5) {
	}
	if(var5 != var4) {
	this->worldObj->markBlocksDirtyVertical(var1, var3, var5, var4);
	this->heightMap[var3 << 4 | var1] = (unsigned char)var5;
	int var7;
	int var8;
	int var9;
	if(var5 < this->heightMapMinimum) {
	this->heightMapMinimum = var5;
	} else {
	var7 = 127;
	for(var8 = 0; var8 < 16; ++var8) {
	for(var9 = 0; var9 < 16; ++var9) {
	if((this->heightMap[var9 << 4 | var8] & 255) < var7) {
	var7 = this->heightMap[var9 << 4 | var8] & 255;
	}
	}
	}
	this->heightMapMinimum = var7;
	}
	var7 = this->xPosition * 16 + var1;
	var8 = this->zPosition * 16 + var3;
	if(var5 < var4) {
	for(var9 = var5; var9 < var4; ++var9) {
	this->skylightMap->setNibble(var1, var9, var3, 15);
	}
	} else {
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Sky, var7, var4, var8, var7, var5, var8);
	for(var9 = var4; var9 < var5; ++var9) {
	this->skylightMap->setNibble(var1, var9, var3, 0);
	}
	}
	var9 = 15;
	int var10;
	for(var10 = var5; var5 > 0 && var9 > 0; this->skylightMap->setNibble(var1, var5, var3, var9)) {
	--var5;
	int var11 = Block::s_lightOpacity[this->getBlockID(var1, var5, var3)];
	if(var11 == 0) {
	var11 = 1;
	}
	var9 -= var11;
	if(var9 < 0) {
	var9 = 0;
	}
	}
	while(var5 > 0 && Block::s_lightOpacity[this->getBlockID(var1, var5 - 1, var3)] == 0) {
	--var5;
	}
	if(var5 != var10) {
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Sky, var7 - 1, var5, var8 - 1, var7 + 1, var10, var8 + 1);
	}
	this->isModified = true;
	}
}

int Chunk::getBlockID(int var1, int var2, int var3)
{
	return this->blocks[var1 << 11 | var3 << 7 | var2];
}

bool Chunk::setBlockIDWithMetadata(int var1, int var2, int var3, int var4, int var5)
{
	unsigned char var6 = (unsigned char)var4;
	int var7 = this->heightMap[var3 << 4 | var1] & 255;
	int var8 = this->blocks[var1 << 11 | var3 << 7 | var2] & 255;
	if(var8 == var4 && this->data->getNibble(var1, var2, var3) == var5) {
	return false;
	} else {
	int var9 = this->xPosition * 16 + var1;
	int var10 = this->zPosition * 16 + var3;
	this->blocks[var1 << 11 | var3 << 7 | var2] = var6;
	if(var8 != 0 && !this->worldObj->multiplayerWorld) {
	if (Block::s_blocksList[var8] != NULL) { Block::s_blocksList[var8]->onBlockRemoval(this->worldObj, var9, var2, var10); }
	}
	this->data->setNibble(var1, var2, var3, var5);
	if(!this->worldObj->worldProvider->hasNoSky) {
	if(Block::s_lightOpacity[var6] != 0) {
	if(var2 >= var7) {
	this->relightBlock(var1, var2 + 1, var3);
	}
	} else if(var2 == var7 - 1) {
	this->relightBlock(var1, var2, var3);
	}
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Sky, var9, var2, var10, var9, var2, var10);
	}
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Block, var9, var2, var10, var9, var2, var10);
	this->checkSkylightNeighbors(var1, var3);
	if(var4 != 0) {
	if (Block::s_blocksList[var4] != NULL) { Block::s_blocksList[var4]->onBlockAdded(this->worldObj, var9, var2, var10); }
	}
	this->data->setNibble(var1, var2, var3, var5);
	this->isModified = true;
	return true;
	}
}

bool Chunk::setBlockID(int var1, int var2, int var3, int var4)
{
	unsigned char var5 = (unsigned char)var4;
	int var6 = this->heightMap[var3 << 4 | var1] & 255;
	int var7 = this->blocks[var1 << 11 | var3 << 7 | var2] & 255;
	if(var7 == var4) {
	return false;
	} else {
	int var8 = this->xPosition * 16 + var1;
	int var9 = this->zPosition * 16 + var3;
	this->blocks[var1 << 11 | var3 << 7 | var2] = var5;
	if(var7 != 0) {
	if (Block::s_blocksList[var7] != NULL) { Block::s_blocksList[var7]->onBlockRemoval(this->worldObj, var8, var2, var9); }
	}
	this->data->setNibble(var1, var2, var3, 0);
	if(Block::s_lightOpacity[var5] != 0) {
	if(var2 >= var6) {
	this->relightBlock(var1, var2 + 1, var3);
	}
	} else if(var2 == var6 - 1) {
	this->relightBlock(var1, var2, var3);
	}
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Sky, var8, var2, var9, var8, var2, var9);
	this->worldObj->scheduleLightingUpdate(EnumSkyBlock::Block, var8, var2, var9, var8, var2, var9);
	this->checkSkylightNeighbors(var1, var3);
	if(var4 != 0 && !this->worldObj->multiplayerWorld) {
	if (Block::s_blocksList[var4] != NULL) { Block::s_blocksList[var4]->onBlockAdded(this->worldObj, var8, var2, var9); }
	}
	this->isModified = true;
	return true;
	}
}

int Chunk::getBlockMetadata(int var1, int var2, int var3)
{
	return this->data->getNibble(var1, var2, var3);
}

void Chunk::setBlockMetadata(int var1, int var2, int var3, int var4)
{
	this->isModified = true;
	this->data->setNibble(var1, var2, var3, var4);
}

int Chunk::getSavedLightValue(int var1, int var2, int var3, int var4)
{
	return var1 == EnumSkyBlock::Sky ? this->skylightMap->getNibble(var2, var3, var4) : (var1 == EnumSkyBlock::Block ? this->blocklightMap->getNibble(var2, var3, var4) : 0);
}

void Chunk::setLightValue(int var1, int var2, int var3, int var4, int var5)
{
	this->isModified = true;
	if(var1 == EnumSkyBlock::Sky) {
	this->skylightMap->setNibble(var2, var3, var4, var5);
	} else {
	if(var1 != EnumSkyBlock::Block) {
	return;
	}
	this->blocklightMap->setNibble(var2, var3, var4, var5);
	}
}

int Chunk::getBlockLightValue(int var1, int var2, int var3, int var4)
{
	int var5 = this->skylightMap->getNibble(var1, var2, var3);
	if(var5 > 0) {
	s_isLit = true;
	}
	var5 -= var4;
	int var6 = this->blocklightMap->getNibble(var1, var2, var3);
	if(var6 > var5) {
	var5 = var6;
	}
	return var5;
}

void Chunk::addEntity(Entity* var1)
{
	if(!this->isEmptyChunk) {
	this->hasEntities = true;
	int var2 = MathHelper::floor_double(var1->posX / 16.0);
	int var3 = MathHelper::floor_double(var1->posZ / 16.0);
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  if(var2 != this.xPosition || var3 != this.zPosition) {
	 *      System.out.println("Wrong location! " + var1);
	 *      Thread.dumpStack();
	 *  }
	 *  // Thread.dumpStack() prints the current thread's stack trace to stderr. The
	 *  // C++ port drops it (no-op); the diagnostic stack dump is lost.
	 *  ---- end original Java ---- */
	if(var2 != this->xPosition || var3 != this->zPosition) {
	System::out.println((JCat() + "Wrong location! " + var1));
	// Thread.dumpStack() (diagnostic only; absent from the Thread runtime stub) -> no-op port.
	}
	int var4 = MathHelper::floor_double(var1->posY / 16.0);
	if(var4 < 0) {
	var4 = 0;
	}
	if(var4 >= this->entities.length) {
	var4 = this->entities.length - 1;
	}
	var1->addedToChunk = true;
	var1->chunkCoordX = this->xPosition;
	var1->chunkCoordY = var4;
	var1->chunkCoordZ = this->zPosition;
	this->entities[var4].push_back(var1);
	}
}

void Chunk::removeEntity(Entity* var1)
{
	this->removeEntityAtIndex(var1, var1->chunkCoordY);
}

void Chunk::removeEntityAtIndex(Entity* var1, int var2)
{
	if(var2 < 0) {
	var2 = 0;
	}
	if(var2 >= this->entities.length) {
	var2 = this->entities.length - 1;
	}
	this->entities[var2].remove(var1);
}

bool Chunk::canBlockSeeTheSky(int var1, int var2, int var3)
{
	return var2 >= (this->heightMap[var3 << 4 | var1] & 255);
}

TileEntity* Chunk::getChunkBlockTileEntity(int var1, int var2, int var3)
{
	ChunkPosition* var4 = new ChunkPosition(var1, var2, var3);
	TileEntity* var5 = (TileEntity*)this->chunkTileEntityMap[_cpKey(var4)];
	if(var5 == NULL) {
	int var6 = this->getBlockID(var1, var2, var3);
	if(!Block::s_isBlockContainer[var6]) {
	return NULL;
	}
	BlockContainer* var7 = (BlockContainer*)Block::s_blocksList[var6];
	var7->onBlockAdded(this->worldObj, this->xPosition * 16 + var1, var2, this->zPosition * 16 + var3);
	var5 = (TileEntity*)this->chunkTileEntityMap[_cpKey(var4)];
	}
	return var5;
}

void Chunk::addTileEntity(TileEntity* var1)
{
	int var2 = var1->xCoord - this->xPosition * 16;
	int var3 = var1->yCoord;
	int var4 = var1->zCoord - this->zPosition * 16;
	this->setChunkBlockTileEntity(var2, var3, var4, var1);
}

void Chunk::setChunkBlockTileEntity(int var1, int var2, int var3, TileEntity* var4)
{
	ChunkPosition* var5 = new ChunkPosition(var1, var2, var3);
	var4->worldObj = this->worldObj;
	var4->xCoord = this->xPosition * 16 + var1;
	var4->yCoord = var2;
	var4->zCoord = this->zPosition * 16 + var3;
	if(this->getBlockID(var1, var2, var3) != 0 && (dynamic_cast<BlockContainer*>(Block::s_blocksList[this->getBlockID(var1, var2, var3)]) != NULL)) {
	if(this->isChunkLoaded) {
	if(this->chunkTileEntityMap[_cpKey(var5)] != NULL) {
	this->worldObj->loadedTileEntityList.remove(this->chunkTileEntityMap[_cpKey(var5)]);
	}
	this->worldObj->loadedTileEntityList.push_back(var4);
	}
	this->chunkTileEntityMap[_cpKey(var5)] = var4;
	} else {
	System::out.println("Attempted to place a tile entity where there was no entity tile!");
	}
}

void Chunk::removeChunkBlockTileEntity(int var1, int var2, int var3)
{
	ChunkPosition* var4 = new ChunkPosition(var1, var2, var3);
	if(this->isChunkLoaded) {
	// Java: Map.remove(key) returns the removed value, then List.remove(value).
	std::map<long long, void*>::iterator var5 = this->chunkTileEntityMap.find(_cpKey(var4));
	if(var5 != this->chunkTileEntityMap.end()) {
	void* var6 = var5->second;
	this->chunkTileEntityMap.erase(var5);
	this->worldObj->loadedTileEntityList.remove(var6);
	}
	}
}

void Chunk::onChunkLoad()
{
	this->isChunkLoaded = true;
	// Java: loadedTileEntityList.addAll(chunkTileEntityMap.values())
	for(std::map<long long, void*>::iterator var0 = this->chunkTileEntityMap.begin(); var0 != this->chunkTileEntityMap.end(); ++var0) {
	this->worldObj->loadedTileEntityList.push_back(var0->second);
	}
	for(int var1 = 0; var1 < this->entities.length; ++var1) {
	this->worldObj->addLoadedEntities(this->entities[var1]);
	}
}

void Chunk::onChunkUnload()
{
	this->isChunkLoaded = false;
	// Java: loadedTileEntityList.removeAll(chunkTileEntityMap.values())
	for(std::map<long long, void*>::iterator var0 = this->chunkTileEntityMap.begin(); var0 != this->chunkTileEntityMap.end(); ++var0) {
	this->worldObj->loadedTileEntityList.remove(var0->second);
	}
	for(int var1 = 0; var1 < this->entities.length; ++var1) {
	this->worldObj->unloadEntities(this->entities[var1]);
	}
}

void Chunk::setChunkModified()
{
	this->isModified = true;
}

void Chunk::getEntitiesWithinAABBForEntity(Entity* var1, AxisAlignedBB* var2, JList<void*>& var3)
{
	int var4 = MathHelper::floor_double((var2->minY - 2.0) / 16.0);
	int var5 = MathHelper::floor_double((var2->maxY + 2.0) / 16.0);
	if(var4 < 0) {
	var4 = 0;
	}
	if(var5 >= this->entities.length) {
	var5 = this->entities.length - 1;
	}
	for(int var6 = var4; var6 <= var5; ++var6) {
	JList<void*> var7 = this->entities[var6];
	for(int var8 = 0; var8 < var7.size(); ++var8) {
	Entity* var9 = (Entity*)var7[var8];
	if(var9 != var1 && var9->boundingBox->intersectsWith(var2)) {
	var3.push_back(var9);
	}
	}
	}
}

void Chunk::getEntitiesOfTypeWithinAAAB(Class* var1, AxisAlignedBB* var2, JList<void*>& var3)
{
	int var4 = MathHelper::floor_double((var2->minY - 2.0) / 16.0);
	int var5 = MathHelper::floor_double((var2->maxY + 2.0) / 16.0);
	if(var4 < 0) {
	var4 = 0;
	}
	if(var5 >= this->entities.length) {
	var5 = this->entities.length - 1;
	}
	for(int var6 = var4; var6 <= var5; ++var6) {
	JList<void*> var7 = this->entities[var6];
	for(int var8 = 0; var8 < var7.size(); ++var8) {
	Entity* var9 = (Entity*)var7[var8];
	/* ---- ORIGINAL JAVA (stubbed below) ----
	 *  if(var1.isAssignableFrom(var9.getClass()) && var9.boundingBox.intersectsWith(var2)) {
	 *      var3.add(var9);
	 *  }
	 *  // Java tests the requested Class against each entity's RUNTIME type
	 *  // (var9.getClass()). The C++ port passes a default-constructed Class() instead
	 *  // of the entity's class, so the per-entity type filter is not faithful
	 *  // (no real getClass()/RTTI available for the entity).
	 *  ---- end original Java ---- */
	if(var1->isAssignableFrom(Class()) && var9->boundingBox->intersectsWith(var2)) {
	var3.push_back(var9);
	}
	}
	}
}

bool Chunk::needsSaving(bool var1)
{
	return this->neverSave ? false : (this->hasEntities && this->worldObj->worldTime != this->lastSaveTime ? true : this->isModified);
}

int Chunk::setChunkData(JArray<unsigned char > var1, int var2, int var3, int var4, int var5, int var6, int var7, int var8)
{
	int var9;
	int var10;
	int var11;
	int var12;
	for(var9 = var2; var9 < var5; ++var9) {
	for(var10 = var4; var10 < var7; ++var10) {
	var11 = (var9 << 11) | (var10 << 7) | var3;
	var12 = var6 - var3;
	System::arraycopy(var1, var8, this->blocks, var11, var12);
	var8 += var12;
	}
	}
	this->generateHeightMap();
	for(var9 = var2; var9 < var5; ++var9) {
	for(var10 = var4; var10 < var7; ++var10) {
	var11 = ((var9 << 11) | (var10 << 7) | var3) >> 1;
	var12 = (var6 - var3) / 2;
	System::arraycopy(var1, var8, this->data->data, var11, var12);
	var8 += var12;
	}
	}
	for(var9 = var2; var9 < var5; ++var9) {
	for(var10 = var4; var10 < var7; ++var10) {
	var11 = ((var9 << 11) | (var10 << 7) | var3) >> 1;
	var12 = (var6 - var3) / 2;
	System::arraycopy(var1, var8, this->blocklightMap->data, var11, var12);
	var8 += var12;
	}
	}
	for(var9 = var2; var9 < var5; ++var9) {
	for(var10 = var4; var10 < var7; ++var10) {
	var11 = ((var9 << 11) | (var10 << 7) | var3) >> 1;
	var12 = (var6 - var3) / 2;
	System::arraycopy(var1, var8, this->skylightMap->data, var11, var12);
	var8 += var12;
	}
	}
	return var8;
}

Random* Chunk::getRandomWithSeed(long long var1)
{
	return new Random(this->worldObj->randomSeed + (long long)(this->xPosition * this->xPosition * 4987142) + (long long)(this->xPosition * 5947611) + (long long)(this->zPosition * this->zPosition) * 4392871LL + (long long)(this->zPosition * 389711) ^ var1);
}
