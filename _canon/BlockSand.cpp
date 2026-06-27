#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSand.h"
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
#include "EntityFallingSand.h"
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

bool BlockSand::s_fallInstantly = false;

BlockSand::BlockSand(int var1, int var2)
	: Block(var1, var2, Material::s_sand)
{
}

BlockSand::~BlockSand()
{
}

void BlockSand::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID);
}

void BlockSand::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID);
}

void BlockSand::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	this->tryToFall(var1, var2, var3, var4);
}

void BlockSand::tryToFall(World* var1, int var2, int var3, int var4)
{
	if(canFallBelow(var1, var2, var3 - 1, var4) && var3 >= 0) {
	EntityFallingSand* var8 = new EntityFallingSand(var1, (float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F, this->blockID);
	if(s_fallInstantly) {
	while(!var8->isDead) {
	var8->onUpdate();
	}
	} else {
	var1->entityJoinedWorld(var8);
	}
	}
}

int BlockSand::tickRate()
{
	return 3;
}

bool BlockSand::canFallBelow(World* var0, int var1, int var2, int var3)
{
	int var4 = var0->getBlockId(var1, var2, var3);
	if(var4 == 0) {
	return true;
	} else if(var4 == Block::s_fire->blockID) {
	return true;
	} else {
	Material* var5 = Block::s_blocksList[var4]->blockMaterial;
	return var5 == Material::s_water ? true : var5 == Material::s_lava;
	}
}

void BlockSand::initStatics()
{
}
