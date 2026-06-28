#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkProviderLoadOrGenerate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkProviderLoadOrGenerate.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IOException.h"
#include "IProgressUpdate.h"
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

bool ChunkProviderLoadOrGenerate::s_deferPopulate = false;

ChunkProviderLoadOrGenerate::ChunkProviderLoadOrGenerate(World* var1, IChunkLoader* var2, IChunkProvider* var3)
	: blankChunk(NULL)
	, chunkProvider(NULL)
	, chunkLoader(NULL)
	, worldObj(NULL)
	, lastQueriedChunkXPos(0)
	, lastQueriedChunkZPos(0)
	, lastQueriedChunk(NULL)
{
	this->chunks = JArray<Chunk* >(1024);
	this->lastQueriedChunkXPos = -999999999;
	this->lastQueriedChunkZPos = -999999999;
	this->blankChunk = new Chunk(var1, JArray<unsigned char >(-Short::MIN_VALUE), 0, 0);
	this->blankChunk->isEmptyChunk = true;
	this->blankChunk->neverSave = true;
	this->worldObj = var1;
	this->chunkLoader = var2;
	this->chunkProvider = var3;
}

ChunkProviderLoadOrGenerate::~ChunkProviderLoadOrGenerate()
{
	delete blankChunk;
	blankChunk = NULL;
}

bool ChunkProviderLoadOrGenerate::chunkExists(int var1, int var2)
{
	if(var1 == this->lastQueriedChunkXPos && var2 == this->lastQueriedChunkZPos && this->lastQueriedChunk != NULL) {
	return true;
	} else {
	int var3 = var1 & 31;
	int var4 = var2 & 31;
	int var5 = var3 + var4 * 32;
	return this->chunks[var5] != NULL && (this->chunks[var5] == this->blankChunk || this->chunks[var5]->isAtLocation(var1, var2));
	}
}

Chunk* ChunkProviderLoadOrGenerate::provideChunk(int var1, int var2)
{
	if(var1 == this->lastQueriedChunkXPos && var2 == this->lastQueriedChunkZPos && this->lastQueriedChunk != NULL) {
	return this->lastQueriedChunk;
	} else {
	int var3 = var1 & 31;
	int var4 = var2 & 31;
	int var5 = var3 + var4 * 32;
	if(!this->chunkExists(var1, var2)) {
	if(this->chunks[var5] != NULL) {
	if (this->chunks[var5] != NULL) { this->chunks[var5]->onChunkUnload(); }
	this->saveChunk(this->chunks[var5]);
	this->saveExtraChunkData(this->chunks[var5]);
	}
	Chunk* var6 = this->loadChunk(var1, var2);
	if(var6 == NULL) {
	if(this->chunkProvider == NULL) {
	var6 = this->blankChunk;
	} else {
	var6 = this->chunkProvider->provideChunk(var1, var2);
	}
	}
	this->chunks[var5] = var6;
	var6->generateBlocklightMap();
	if(this->chunks[var5] != NULL) {
	if (this->chunks[var5] != NULL) { this->chunks[var5]->onChunkLoad(); }
	}
	if(!ChunkProviderLoadOrGenerate::s_deferPopulate) {   // skip the populate CASCADE during pre-gen
	if(!this->chunks[var5]->isTerrainPopulated && this->chunkExists(var1 + 1, var2 + 1) && this->chunkExists(var1, var2 + 1) && this->chunkExists(var1 + 1, var2)) {
	this->populate(this, var1, var2);
	}
	if(this->chunkExists(var1 - 1, var2) && !this->provideChunk(var1 - 1, var2)->isTerrainPopulated && this->chunkExists(var1 - 1, var2 + 1) && this->chunkExists(var1, var2 + 1) && this->chunkExists(var1 - 1, var2)) {
	this->populate(this, var1 - 1, var2);
	}
	if(this->chunkExists(var1, var2 - 1) && !this->provideChunk(var1, var2 - 1)->isTerrainPopulated && this->chunkExists(var1 + 1, var2 - 1) && this->chunkExists(var1, var2 - 1) && this->chunkExists(var1 + 1, var2)) {
	this->populate(this, var1, var2 - 1);
	}
	if(this->chunkExists(var1 - 1, var2 - 1) && !this->provideChunk(var1 - 1, var2 - 1)->isTerrainPopulated && this->chunkExists(var1 - 1, var2 - 1) && this->chunkExists(var1, var2 - 1) && this->chunkExists(var1 - 1, var2)) {
	this->populate(this, var1 - 1, var2 - 1);
	}
	}
	}
	this->lastQueriedChunkXPos = var1;
	this->lastQueriedChunkZPos = var2;
	this->lastQueriedChunk = this->chunks[var5];
	return this->chunks[var5];
	}
}

Chunk* ChunkProviderLoadOrGenerate::loadChunk(int var1, int var2)
{
	if(this->chunkLoader == NULL) {
	return NULL;
	} else {
	try {
	Chunk* var3 = this->chunkLoader->loadChunk(this->worldObj, var1, var2);
	if(var3 != NULL) {
	var3->lastSaveTime = this->worldObj->worldTime;
	}
	return var3;
	} catch (Exception* var4) {
	var4->printStackTrace();
	return NULL;
	}
	}
}

void ChunkProviderLoadOrGenerate::saveExtraChunkData(Chunk* var1)
{
	if(this->chunkLoader != NULL) {
	try {
	this->chunkLoader->saveExtraChunkData(this->worldObj, var1);
	} catch (Exception* var3) {
	var3->printStackTrace();
	}
	}
}

void ChunkProviderLoadOrGenerate::saveChunk(Chunk* var1)
{
	if(this->chunkLoader != NULL) {
	try {
	var1->lastSaveTime = this->worldObj->worldTime;
	this->chunkLoader->saveChunk(this->worldObj, var1);
	} catch (IOException* var3) {
	var3->printStackTrace();
	}
	}
}

void ChunkProviderLoadOrGenerate::populate(IChunkProvider* var1, int var2, int var3)
{
	Chunk* var4 = this->provideChunk(var2, var3);
	if(!var4->isTerrainPopulated) {
	var4->isTerrainPopulated = true;
	if(this->chunkProvider != NULL) {
	this->chunkProvider->populate(var1, var2, var3);
	var4->setChunkModified();
	}
	}
}

bool ChunkProviderLoadOrGenerate::saveChunks(bool var1, IProgressUpdate* var2)
{
	int var3 = 0;
	int var4 = 0;
	int var5;
	if(var2 != NULL) {
	for(var5 = 0; var5 < this->chunks.length; ++var5) {
	if(this->chunks[var5] != NULL && this->chunks[var5]->needsSaving(var1)) {
	++var4;
	}
	}
	}
	var5 = 0;
	for(int var6 = 0; var6 < this->chunks.length; ++var6) {
	if(this->chunks[var6] != NULL) {
	if(var1 && !this->chunks[var6]->neverSave) {
	this->saveExtraChunkData(this->chunks[var6]);
	}
	if(this->chunks[var6]->needsSaving(var1)) {
	this->saveChunk(this->chunks[var6]);
	this->chunks[var6]->isModified = false;
	++var3;
	if(var3 == 2 && !var1) {
	return false;
	}
	if(var2 != NULL) {
	++var5;
	if(var5 % 10 == 0) {
	var2->setLoadingProgress(var5 * 100 / var4);
	}
	}
	}
	}
	}
	if(var1) {
	if(this->chunkLoader == NULL) {
	return true;
	}
	this->chunkLoader->saveExtraData();
	}
	return true;
}

bool ChunkProviderLoadOrGenerate::unloadQueuedChunks()
{
	if(this->chunkLoader != NULL) {
	this->chunkLoader->func_814_a();
	}
	return this->chunkProvider->unloadQueuedChunks();
}

bool ChunkProviderLoadOrGenerate::canSave()
{
	return true;
}

void ChunkProviderLoadOrGenerate::initStatics()
{
}
