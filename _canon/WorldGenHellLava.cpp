#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenHellLava.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenHellLava.h"
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
#include "WorldGenerator.h"
#include "WorldProvider.h"

WorldGenHellLava::WorldGenHellLava(int var1)
	: liquidBlockId(0)
{
	this->liquidBlockId = var1;
}

WorldGenHellLava::~WorldGenHellLava()
{
}

bool WorldGenHellLava::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	if(var1->getBlockId(var3, var4 + 1, var5) != Block::s_bloodStone->blockID) {
	return false;
	} else if(var1->getBlockId(var3, var4, var5) != 0 && var1->getBlockId(var3, var4, var5) != Block::s_bloodStone->blockID) {
	return false;
	} else {
	int var6 = 0;
	if(var1->getBlockId(var3 - 1, var4, var5) == Block::s_bloodStone->blockID) {
	++var6;
	}
	if(var1->getBlockId(var3 + 1, var4, var5) == Block::s_bloodStone->blockID) {
	++var6;
	}
	if(var1->getBlockId(var3, var4, var5 - 1) == Block::s_bloodStone->blockID) {
	++var6;
	}
	if(var1->getBlockId(var3, var4, var5 + 1) == Block::s_bloodStone->blockID) {
	++var6;
	}
	if(var1->getBlockId(var3, var4 - 1, var5) == Block::s_bloodStone->blockID) {
	++var6;
	}
	int var7 = 0;
	if(var1->getBlockId(var3 - 1, var4, var5) == 0) {
	++var7;
	}
	if(var1->getBlockId(var3 + 1, var4, var5) == 0) {
	++var7;
	}
	if(var1->getBlockId(var3, var4, var5 - 1) == 0) {
	++var7;
	}
	if(var1->getBlockId(var3, var4, var5 + 1) == 0) {
	++var7;
	}
	if(var1->getBlockId(var3, var4 - 1, var5) == 0) {
	++var7;
	}
	if(var6 == 4 && var7 == 1) {
	var1->setBlockWithNotify(var3, var4, var5, this->liquidBlockId);
	var1->scheduledUpdatesAreImmediate = true;
	if (Block::s_blocksList[this->liquidBlockId] != NULL) { Block::s_blocksList[this->liquidBlockId]->updateTick(var1, var3, var4, var5, var2); }
	var1->scheduledUpdatesAreImmediate = false;
	}
	return true;
	}
}

void WorldGenHellLava::initStatics()
{
}
