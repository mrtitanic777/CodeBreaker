#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenLightStone1.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenLightStone1.h"
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

WorldGenLightStone1::~WorldGenLightStone1()
{
}

bool WorldGenLightStone1::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	if(var1->getBlockId(var3, var4, var5) != 0) {
	return false;
	} else if(var1->getBlockId(var3, var4 + 1, var5) != Block::s_bloodStone->blockID) {
	return false;
	} else {
	var1->setBlockWithNotify(var3, var4, var5, Block::s_lightStone->blockID);
	for(int var6 = 0; var6 < 1500; ++var6) {
	int var7 = var3 + var2->nextInt(8) - var2->nextInt(8);
	int var8 = var4 - var2->nextInt(12);
	int var9 = var5 + var2->nextInt(8) - var2->nextInt(8);
	if(var1->getBlockId(var7, var8, var9) == 0) {
	int var10 = 0;
	for(int var11 = 0; var11 < 6; ++var11) {
	int var12 = 0;
	if(var11 == 0) {
	var12 = var1->getBlockId(var7 - 1, var8, var9);
	}
	if(var11 == 1) {
	var12 = var1->getBlockId(var7 + 1, var8, var9);
	}
	if(var11 == 2) {
	var12 = var1->getBlockId(var7, var8 - 1, var9);
	}
	if(var11 == 3) {
	var12 = var1->getBlockId(var7, var8 + 1, var9);
	}
	if(var11 == 4) {
	var12 = var1->getBlockId(var7, var8, var9 - 1);
	}
	if(var11 == 5) {
	var12 = var1->getBlockId(var7, var8, var9 + 1);
	}
	if(var12 == Block::s_lightStone->blockID) {
	++var10;
	}
	}
	if(var10 == 1) {
	var1->setBlockWithNotify(var7, var8, var9, Block::s_lightStone->blockID);
	}
	}
	}
	return true;
	}
}

void WorldGenLightStone1::initStatics()
{
}
