#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from WorldGenLakes.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenLakes.h"
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
#include "EnumSkyBlock.h"
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

WorldGenLakes::WorldGenLakes(int var1)
	: blockIndex(0)
{
	this->blockIndex = var1;
}

WorldGenLakes::~WorldGenLakes()
{
}

bool WorldGenLakes::generate(World* var1, Random* var2, int var3, int var4, int var5)
{
	var3 -= 8;
	for(var5 -= 8; var4 > 0 && var1->getBlockId(var3, var4, var5) == 0; --var4) {
	}
	var4 -= 4;
	JArray<bool > var6 = JArray<bool >(2048);
	int var7 = var2->nextInt(4) + 4;
	int var8;
	for(var8 = 0; var8 < var7; ++var8) {
	double var9 = var2->nextDouble() * 6.0 + 3.0;
	double var11 = var2->nextDouble() * 4.0 + 2.0;
	double var13 = var2->nextDouble() * 6.0 + 3.0;
	double var15 = var2->nextDouble() * (16.0 - var9 - 2.0) + 1.0 + var9 / 2.0;
	double var17 = var2->nextDouble() * (8.0 - var11 - 4.0) + 2.0 + var11 / 2.0;
	double var19 = var2->nextDouble() * (16.0 - var13 - 2.0) + 1.0 + var13 / 2.0;
	for(int var21 = 1; var21 < 15; ++var21) {
	for(int var22 = 1; var22 < 15; ++var22) {
	for(int var23 = 1; var23 < 7; ++var23) {
	double var24 = ((double)var21 - var15) / (var9 / 2.0);
	double var26 = ((double)var23 - var17) / (var11 / 2.0);
	double var28 = ((double)var22 - var19) / (var13 / 2.0);
	double var30 = var24 * var24 + var26 * var26 + var28 * var28;
	if(var30 < 1.0) {
	var6[(var21 * 16 + var22) * 8 + var23] = true;
	}
	}
	}
	}
	}
	int var10;
	int var32;
	for(var8 = 0; var8 < 16; ++var8) {
	for(var32 = 0; var32 < 16; ++var32) {
	for(var10 = 0; var10 < 8; ++var10) {
	bool var33 = !var6[(var8 * 16 + var32) * 8 + var10] && (var8 < 15 && var6[((var8 + 1) * 16 + var32) * 8 + var10] || var8 > 0 && var6[((var8 - 1) * 16 + var32) * 8 + var10] || var32 < 15 && var6[(var8 * 16 + var32 + 1) * 8 + var10] || var32 > 0 && var6[(var8 * 16 + (var32 - 1)) * 8 + var10] || var10 < 7 && var6[(var8 * 16 + var32) * 8 + var10 + 1] || var10 > 0 && var6[(var8 * 16 + var32) * 8 + (var10 - 1)]);
	if(var33) {
	Material* var12 = var1->getBlockMaterial(var3 + var8, var4 + var10, var5 + var32);
	if(var10 >= 4 && var12->getIsLiquid()) {
	return false;
	}
	if(var10 < 4 && !var12->isSolid() && var1->getBlockId(var3 + var8, var4 + var10, var5 + var32) != this->blockIndex) {
	return false;
	}
	}
	}
	}
	}
	for(var8 = 0; var8 < 16; ++var8) {
	for(var32 = 0; var32 < 16; ++var32) {
	for(var10 = 0; var10 < 8; ++var10) {
	if(var6[(var8 * 16 + var32) * 8 + var10]) {
	var1->setBlockWithNotify(var3 + var8, var4 + var10, var5 + var32, var10 >= 4 ? 0 : this->blockIndex);
	}
	}
	}
	}
	for(var8 = 0; var8 < 16; ++var8) {
	for(var32 = 0; var32 < 16; ++var32) {
	for(var10 = 4; var10 < 8; ++var10) {
	if(var6[(var8 * 16 + var32) * 8 + var10] && var1->getBlockId(var3 + var8, var4 + var10 - 1, var5 + var32) == Block::s_dirt->blockID && var1->getSavedLightValue(EnumSkyBlock::Sky, var3 + var8, var4 + var10, var5 + var32) > 0) {
	var1->setBlockWithNotify(var3 + var8, var4 + var10 - 1, var5 + var32, Block::s_grass->blockID);
	}
	}
	}
	}
	return true;
}

void WorldGenLakes::initStatics()
{
}
